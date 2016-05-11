#include "StdAfx.h"
#include "USN.h"

bool EnumUsnRecord( TCHAR volume_id, EnumUsnRecordProc enum_proc, LPVOID user_data )
{
	bool ret = false;

	TCHAR	FileSystemName[MAX_PATH+1];
	DWORD	MaximumComponentLength;

	TCHAR	RootPathName[] = TEXT("x:\\");
	RootPathName[0] = volume_id;

	if(!GetVolumeInformation(RootPathName, 0, 0, 0, &MaximumComponentLength, 0, FileSystemName, MAX_PATH + 1))
		return false;

	if( 0 !=_tcscmp(FileSystemName, TEXT("NTFS")))
		return false;

	TCHAR	UsnFileName[] = TEXT("\\\\.\\x:");
	UsnFileName[4] = volume_id;

	HANDLE hVol = CreateFile(UsnFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if( hVol != INVALID_HANDLE_VALUE && NULL != hVol )
	{
		DWORD bytes_returned;
		CREATE_USN_JOURNAL_DATA cujd = { 0, 0 };	// 默认大小

		if( DeviceIoControl( hVol, FSCTL_CREATE_USN_JOURNAL, &cujd, sizeof(cujd), NULL, 0, &bytes_returned, NULL ) ) // 如果创建过，且没有用FSCTL_DELETE_USN_JOURNAL关闭，则可以跳过这一步
		{
			USN_JOURNAL_DATA qujd;
			if( DeviceIoControl( hVol, FSCTL_QUERY_USN_JOURNAL, NULL, 0, &qujd, sizeof(qujd), &bytes_returned, NULL ) )
			{
				char buffer[0x1000]; // 缓冲区越大则DeviceIoControl调用次数越少，即效率越高
				DWORD BytesReturned;

				// 使用FSCTL_ENUM_USN_DATA可以列出所有存在的文件信息，但UsnRecord->Reason等信息是无效的
				MFT_ENUM_DATA med = { 0, 0, qujd.NextUsn };
				for( ; DeviceIoControl(hVol,FSCTL_ENUM_USN_DATA, &med, sizeof(med), buffer, ARRAYSIZE(buffer), &BytesReturned, NULL); med.StartFileReferenceNumber=*(USN*)&buffer )
				{
					const char* buffer_end = buffer + BytesReturned - sizeof(USN_RECORD);
					for(const char* p = buffer + sizeof(USN); p <= buffer_end; p += ((PUSN_RECORD)p)->RecordLength)
					{
						PUSN_RECORD record = (PUSN_RECORD)p;  
						enum_proc(record, user_data);
					}
				}

				ret = true;
			}
				
			DELETE_USN_JOURNAL_DATA dujd = { qujd.UsnJournalID, USN_DELETE_FLAG_DELETE };
			DeviceIoControl( hVol, FSCTL_DELETE_USN_JOURNAL, &dujd, sizeof(dujd), NULL, 0, &bytes_returned, NULL ); // 关闭USN记录。如果是别人的电脑，当然可以不关^_^
		}
		CloseHandle( hVol );
	}

	return ret;
}

UsnRecordEnumerator::UsnRecordEnumerator() 
	: current_record_inbuffer_(0)
{}



bool UsnRecordEnumerator::Initialize(TCHAR volume_id)
{
	if(!usn_buffers_.empty())
	{
		Destroy();
	}

	Reset();

	bool ret = false;

	TCHAR	FileSystemName[MAX_PATH+1];
	DWORD	MaximumComponentLength;

	TCHAR	RootPathName[] = TEXT("x:\\");
	RootPathName[0] = volume_id;

	if(!GetVolumeInformation(RootPathName, 0, 0, 0, &MaximumComponentLength, 0, FileSystemName, MAX_PATH + 1))
		return false;

	if( 0 !=_tcscmp(FileSystemName, TEXT("NTFS")))
		return false;

	TCHAR	UsnFileName[] = TEXT("\\\\.\\x:");
	UsnFileName[4] = volume_id;

	HANDLE hVol = CreateFile(UsnFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if( hVol != INVALID_HANDLE_VALUE && NULL != hVol )
	{
		DWORD bytes_returned;
		CREATE_USN_JOURNAL_DATA cujd = { 0, 0 };	// 默认大小

		if( DeviceIoControl( hVol, FSCTL_CREATE_USN_JOURNAL, &cujd, sizeof(cujd), NULL, 0, &bytes_returned, NULL ) ) // 如果创建过，且没有用FSCTL_DELETE_USN_JOURNAL关闭，则可以跳过这一步
		{
			USN_JOURNAL_DATA qujd;
			if( DeviceIoControl( hVol, FSCTL_QUERY_USN_JOURNAL, NULL, 0, &qujd, sizeof(qujd), &bytes_returned, NULL ) )
			{
				DWORD BytesReturned;
				// 使用FSCTL_ENUM_USN_DATA可以列出所有存在的文件信息，但UsnRecord->Reason等信息是无效的
				MFT_ENUM_DATA med = { 0, 0, qujd.NextUsn };
				for(LPVOID buf = malloc(0x1000); DeviceIoControl(hVol, FSCTL_ENUM_USN_DATA, &med, sizeof(med), buf, 0x1000, &BytesReturned, NULL); buf = malloc(0x1000))
				{
					usn_buffers_.push_back(BufferPage((const char*)buf, BytesReturned));
					med.StartFileReferenceNumber=*(USN*)&buf;
				}

				ret = true;
			}

			DELETE_USN_JOURNAL_DATA dujd = { qujd.UsnJournalID, USN_DELETE_FLAG_DELETE };
			DeviceIoControl( hVol, FSCTL_DELETE_USN_JOURNAL, &dujd, sizeof(dujd), NULL, 0, &bytes_returned, NULL ); // 关闭USN记录。如果是别人的电脑，当然可以不关^_^
		}
		CloseHandle( hVol );
	}

	return ret;
}

void UsnRecordEnumerator::Reset()
{
	current_record_inbuffer_ = 0;
}

void UsnRecordEnumerator::Destroy()
{
	for (BufferPageList::iterator i = usn_buffers_.begin(); i != usn_buffers_.end(); ++i)
	{
		free((void*)i->buffer);
	}

	usn_buffers_.clear();
}

UsnRecordEnumerator::~UsnRecordEnumerator()
{
	if(!usn_buffers_.empty())
	{
		Destroy();
	}
}

bool UsnRecordEnumerator::MoveNext( PUSN_RECORD* record )
{
	if(0 == current_record_inbuffer_)
	{
		if (usn_buffers_.empty())
		{
			return false;
		}
		else
		{
			current_usn_buffer_ = usn_buffers_.begin();
			current_record_inbuffer_ = current_usn_buffer_->buffer + sizeof(USN);
		}
	}
	else
	{
		current_record_inbuffer_ += ((PUSN_RECORD)current_record_inbuffer_)->RecordLength;
		if(current_record_inbuffer_ >= current_usn_buffer_->buffer_end)
		{
			++current_usn_buffer_;
			if(usn_buffers_.end() == current_usn_buffer_)
			{
				return false;
			}
			else
			{
				current_record_inbuffer_ = current_usn_buffer_->buffer + sizeof(USN);
			}
		}
	}

	if(record)
	{
		*record = (PUSN_RECORD)current_record_inbuffer_;
	}

	return true;
}
