#pragma once

struct IPipeMessageHandler
{
	virtual ~IPipeMessageHandler() = 0 {};
	virtual void OnConnectFail() = 0;
	virtual void OnConnectSucceed() = 0;
	virtual void OnDisconnect() = 0;
	virtual void OnMessgeArrived(LPCTSTR msg) = 0;
	virtual void OnRecvBufExtended() = 0;
};

class CPipeDriver
{
	struct MYOVERLAPPED : public OVERLAPPED
	{
		CPipeDriver* pDriver;
	};

	enum	{FLAG_NONE = 0, FLAG_CONNECTING = 1, FLAG_CONNECTED = 2, FLAG_CLOSED = 4, FLAG_CLOSING = 8};
public:
	CPipeDriver(void);
	~CPipeDriver(void);

	bool Start(LPCTSTR pipe_name, bool as_server, IPipeMessageHandler* handler);
	void Stop();
	bool Send(LPCTSTR msg);

protected:
	static void _cdecl	PipeThreadProc(void*);
	static VOID CALLBACK FileIOCP_Recv(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
	static VOID CALLBACK FileIOCP_Send(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

	static VOID NTAPI APCP_Send(ULONG_PTR Parameter);

	bool Run();

	bool StartPipeAsServer(LPCTSTR pipe_name);
	bool StartPipeAsClient(LPCTSTR pipe_name);

	bool ReallocRecvBuf(DWORD new_size);

	inline bool BeClosing();

	void RecvOverlapped(bool append_buf);
	bool WaitConnectionEstablished();


	void OnArrive( DWORD dwNumberOfBytesTransfered );
	

protected:
	HANDLE		pipe_handle_;
	HANDLE		pipe_thread_handle_;
	HANDLE		pipe_connect_event_;
	MYOVERLAPPED	pipe_overlapped_;
	IPipeMessageHandler* pipe_handler_;
	DWORD		flag_;

	char*		recv_buf_;
	DWORD		recv_buf_recvived_;
	DWORD		recv_buf_size_;
};

