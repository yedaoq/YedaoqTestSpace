#include "StdAfx.h"
#include "PipeDriver.h"
#include <Strsafe.h>

struct DataContext4Send : OVERLAPPED
{
	CPipeDriver*	driver;
	LPTSTR			msg;
};

CPipeDriver::CPipeDriver(void)
	: pipe_handle_(INVALID_HANDLE_VALUE), pipe_connect_event_(0), pipe_handler_(0), pipe_thread_handle_(INVALID_HANDLE_VALUE), flag_(FLAG_NONE), recv_buf_size_(0), recv_buf_(0)
{
}

CPipeDriver::~CPipeDriver(void)
{
}

bool CPipeDriver::Start( LPCTSTR pipe_name, bool as_server, IPipeMessageHandler* handler )
{
	if(!pipe_name || !*pipe_name || !handler)
	{
		return false;
	}

	pipe_handler_ = handler;

	// generate whole pipe name
	TCHAR	pipe_name_whole[MAX_PATH] = TEXT("\\\\.\\pipe\\");
	int		pipe_name_suffix_len = _tcslen(pipe_name_whole);
	::StringCchCopy(pipe_name_whole + pipe_name_suffix_len, ARRAYSIZE(pipe_name_whole) - pipe_name_suffix_len - 1, pipe_name);

	memset(&pipe_overlapped_, 0, sizeof(pipe_overlapped_));
	pipe_overlapped_.pDriver = this;

	recv_buf_size_ = 16;
	recv_buf_ = (char*)malloc(recv_buf_size_);
	recv_buf_recvived_ = 0;

	bool succeed = false;
	if(as_server)
	{
		succeed = StartPipeAsServer(pipe_name_whole);
	}
	else
	{
		succeed = StartPipeAsClient(pipe_name_whole);
	}

	if(!succeed)
	{
		return false;
	}

	flag_ = FLAG_CONNECTING;
	pipe_thread_handle_ = (HANDLE)_beginthread(PipeThreadProc, 0, (void*)this);
	if(INVALID_HANDLE_VALUE == pipe_thread_handle_ || 0 == pipe_thread_handle_)
	{
		return false;
	}

	return true;
}

void _cdecl CPipeDriver::PipeThreadProc( void* param)
{
	CPipeDriver* driver = (CPipeDriver*)param;
	driver->Run();
	_endthread();
}

bool CPipeDriver::Run()
{
	if (!WaitConnectionEstablished())
	{
		return false;
	}

	pipe_overlapped_.hEvent = (HANDLE)this;
	
	RecvOverlapped(false);

	while(!BeClosing())
	{
		SleepEx(1000, TRUE);
	}

	return true;
}

bool CPipeDriver::WaitConnectionEstablished()
{
	if(0 == pipe_connect_event_)
		return true;
	
	bool	established = false;
	DWORD	wait_times = 0;
	while (!BeClosing())
	{
		DWORD wait_result = WaitForSingleObject(pipe_connect_event_, 100);
		if(WAIT_OBJECT_0 == wait_result)
		{
			pipe_handler_->OnConnectSucceed();
			established = true;
			break;
		}
		if(++wait_times > 10 * 5 * 3)	// 3 minutes
		{
			pipe_handler_->OnConnectFail();
			break;
		}
	}

	pipe_overlapped_.hEvent = 0;
	CloseHandle(pipe_connect_event_);
	return established;
}

void CPipeDriver::Stop()
{
	flag_ |= FLAG_CLOSING;
	DWORD dwRet = WaitForSingleObject(pipe_thread_handle_, 2000);
	if (WAIT_OBJECT_0 != dwRet)
	{
		// log error
	}
	
	pipe_thread_handle_ = 0;
	flag_ = FLAG_CLOSED;
}

void CPipeDriver::RecvOverlapped(bool append_buf)
{
	if (!append_buf)
	{
		recv_buf_recvived_ = 0;
	}

	if(!ReadFileEx(pipe_handle_, recv_buf_ + recv_buf_recvived_, recv_buf_size_ - recv_buf_recvived_, &pipe_overlapped_, FileIOCP_Recv))
	{
		DWORD dwErr = GetLastError();
		//ERROR_PIPE_BUSY
		int a = dwErr;
	}
}

bool CPipeDriver::StartPipeAsServer( LPCTSTR pipe_name )
{
	pipe_handle_ = CreateNamedPipe(pipe_name,
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		3,
		4086,
		4086,
		5000,
		NULL);

	if(INVALID_HANDLE_VALUE == pipe_handle_ )
	{
		// log error
		return false;
	}

	pipe_connect_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
	pipe_overlapped_.hEvent = pipe_connect_event_;

	if(!ConnectNamedPipe(pipe_handle_, &pipe_overlapped_))
	{
		switch(DWORD dwErr = GetLastError())
		{
		case ERROR_PIPE_CONNECTED:
			SetEvent(pipe_connect_event_);
			break;
		case ERROR_IO_PENDING:
			break;

		default:
			// log error
			return false;
		}
	}
	return true;
}

bool CPipeDriver::StartPipeAsClient( LPCTSTR pipe_name )
{
	pipe_handle_ = CreateFile( 
		pipe_name,   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE, 
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION | FILE_FLAG_OVERLAPPED,              // default attributes 
		NULL);          // no template file 

	// Break if the pipe handle is valid. 
	if (INVALID_HANDLE_VALUE == pipe_handle_) 
	{
		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		if (GetLastError() == ERROR_PIPE_BUSY) 
		{
			// log : pipe has been connected by other client
		}
		pipe_handler_->OnConnectFail();
		return false;
	}

	pipe_handler_->OnConnectSucceed();
	return true;
}

VOID CALLBACK CPipeDriver::FileIOCP_Recv( DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped )
{
	if(ERROR_SUCCESS != dwErrorCode)
	{
		if(ERROR_BROKEN_PIPE == dwErrorCode)
		{
			if(lpOverlapped)
			{
				CPipeDriver* driver = (CPipeDriver*)lpOverlapped->hEvent;
				driver->pipe_handler_->OnDisconnect();
				return;
			}
		}
		else if(ERROR_MORE_DATA == dwErrorCode)
		{
			if(lpOverlapped)
			{
				CPipeDriver* driver = (CPipeDriver*)lpOverlapped->hEvent;
				driver->pipe_handler_->OnDisconnect();

				driver->recv_buf_size_ = driver->recv_buf_size_ << 1;
				void* new_buf = malloc(driver->recv_buf_size_);

				driver->recv_buf_recvived_ += dwNumberOfBytesTransfered;
				memcpy(new_buf, driver->recv_buf_, driver->recv_buf_recvived_);

				void* old_buf = driver->recv_buf_;
				driver->recv_buf_ = (char*)new_buf;

				driver->RecvOverlapped(true);
			}
		}
		// log error
		return;
	}

	if(lpOverlapped)
	{
		CPipeDriver* driver = (CPipeDriver*)lpOverlapped->hEvent;
		driver->OnArrive(dwNumberOfBytesTransfered);
		driver->RecvOverlapped(false);
	}
}

void CPipeDriver::OnArrive( DWORD dwNumberOfBytesTransfered )
{
	pipe_handler_->OnMessgeArrived((LPCTSTR)recv_buf_);
}

bool CPipeDriver::BeClosing()
{
	return 0 != (flag_ & FLAG_CLOSING);
}

bool CPipeDriver::Send( LPCTSTR msg )
{
	if(!msg)
	{
		return false;
	}

	int msg_len = _tcslen(msg);

	DataContext4Send* ctx = new DataContext4Send;
	memset(ctx, 0, sizeof(DataContext4Send));
	ctx->driver = this;
	ctx->msg = (LPTSTR)malloc((msg_len + 1) * sizeof(*msg));
	StringCchCopy(ctx->msg, msg_len + 1, msg);

	QueueUserAPC(APCP_Send, pipe_thread_handle_, (ULONG_PTR)ctx);
	return true;
}

VOID NTAPI CPipeDriver::APCP_Send( ULONG_PTR Parameter )
{
	DataContext4Send* ctx = (DataContext4Send*)Parameter;
	if(!ctx)
	{
		// log error
	}

	if(!WriteFileEx(ctx->driver->pipe_handle_, ctx->msg, (_tcslen(ctx->msg) + 1) * sizeof(*ctx->msg), ctx, FileIOCP_Send))
	{
		DWORD dwErr = GetLastError();
		dwErr = dwErr;
	}
}

VOID CALLBACK CPipeDriver::FileIOCP_Send( DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped )
{
	if(ERROR_SUCCESS != dwErrorCode || !lpOverlapped)
	{
		// log error
	}

	if(lpOverlapped)
	{
		DataContext4Send* ctx = (DataContext4Send*)lpOverlapped;
		free((void*)ctx->msg);
		delete ctx;
	}
}

