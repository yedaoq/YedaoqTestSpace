#pragma once

#include "winioctl.h"
#include <list>

typedef void (*EnumUsnRecordProc)(const USN_RECORD* record, LPVOID user_data);

bool EnumUsnRecord( TCHAR volume_id, EnumUsnRecordProc enum_proc, LPVOID user_data);

class UsnRecordEnumerator
{
public:

	UsnRecordEnumerator();
	~UsnRecordEnumerator();

	bool Initialize(TCHAR volume_id);
	bool MoveNext(PUSN_RECORD* record);

	void Reset();
	void Destroy();

protected:
	struct BufferPage{
		const char*	buffer;
		const char*	buffer_end;

		BufferPage(const char* buf, DWORD bytes_wirtten) : buffer(buf), buffer_end(buf + bytes_wirtten){}
	};


	typedef std::list<BufferPage> BufferPageList;

	BufferPageList				usn_buffers_;
	BufferPageList::iterator	current_usn_buffer_;
	const char*					current_record_inbuffer_;

};