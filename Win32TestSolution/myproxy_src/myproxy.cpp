// myproxy.cpp : Defines the entry point for the console application.
// Author: Like Zhang
// Email: lzhang@cs.utsa.edu
// 12/05/2007

#include "stdafx.h"

#include "winsock2.h"
#include <process.h>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

#define SPACE_CHAR  ' '
#define PROXY_PORT  6666
#define RESPONSE_BUFFER_SIZE 8192
#define MAX_THREADS		50

queue<SOCKET> socketBuffer;
HANDLE queueMutex;
HANDLE threadSymaphore;

SYSTEM_INFO sysInfo;

void GetTarget(const char* request,string& host, string& port);
string GetTargetResponse(int threadid, const char* TargetURL, const char* Port, const char* request,SOCKET& output);

void InitSockAddr(struct sockaddr_in& sockAddr, short family, unsigned long addr, unsigned short port)
{
	sockAddr.sin_family=family;
	sockAddr.sin_addr.s_addr=addr;
	sockAddr.sin_port=port;

};

unsigned __stdcall RequestThread( void * arg )
{
	int *Arg=(int*)arg;
	int tid=*Arg;
	while(true)
	{

	   SOCKET ioSock;

		WaitForSingleObject(queueMutex,INFINITE);
		
		if(socketBuffer.size()<1)
		{
			ReleaseMutex(queueMutex);
			continue;
		}

		try
		{
			ioSock=socketBuffer.front();
			socketBuffer.pop();
			ReleaseMutex(queueMutex);
		}
		catch(exception)
		{
			ReleaseMutex(queueMutex);
			continue;
		}

		try
		{
			const int bufferSize=1024;
			char buf[bufferSize];
			memset(buf,0,bufferSize);
			
			string data="";

			int recvLen=recv(ioSock,buf,bufferSize-1,0);
			if(recvLen<=0)
			{
				return 0;
			}
			data+=buf;

			while(recvLen>=bufferSize-1)
			{
				memset(buf,0,bufferSize);
				recvLen=recv(ioSock,buf,bufferSize-1,0);
				data+=buf;
			}

			string targetURL="";
			string Port="";
			GetTarget(data.c_str(),targetURL,Port);

			GetTargetResponse(tid,targetURL.c_str(),Port.c_str(),data.c_str(),ioSock);
			shutdown(ioSock,2);
			closesocket(ioSock);
			cout<<"Thread "<<tid<<" socket closed"<<endl;
		} 
		catch(exception)
		{
			shutdown(ioSock,2);
			closesocket(ioSock);
		}
	}
		return 1;

}



void GetTarget(const char* request,string& host,string& port)
{
	host="";
	port="";

	//skip the first HTTP command
	while(*request!=' ')
	{
		++request;
	}
	//skip space
	while( (*request<'a' || *request>'z') && (*request<'A' || *request>'Z') )
				++request;
	const char* urlStart=request;

	if(    (*request=='H' || *request=='h')
		&& (*request=='T' || *request=='t')
		&& (*request=='T' || *request=='t')
		&& (*request=='P' || *request=='p')
		&& (*request==':')
		)
	{
		while(SPACE_CHAR==*request)
			++request;

		while(SPACE_CHAR!=*request && ':'!=*request && '\0'!=*request && '\r'!=*request && '\n'!=*request)
		{
			host+=*request;
			++request;
		}
		if(':'==*request)
		{
			while(SPACE_CHAR!=*request && ':'!=*request && '\0'!=*request && '\r'!=*request && '\n'!=*request)
			{
				port+=*request;
				++request;
			}
		}
		if(port.length()==0)
			port="80";

		return;
	}

	while(*request!='\0')
	{
		//Process "Host: "
		if(*request=='\n' && *(request+1)=='H' && *(request+2)=='o' && *(request+3)=='s' && *(request+4)=='t' && *(request+5)==':')
		{
			request+=6;

			while( (*request<'a' || *request>'z') && (*request<'A' || *request>'Z') )
				++request;
			//find host address in "Host: "
			while( *request!=SPACE_CHAR && *request!='\t' && *request!='\r' && *request!='\n' && *request!='\0' && *request!=':')
			{
				host+=*request;
				++request;
			}
			//find port number in "Host: "
			if(*request==':')
			{
				while( *request!=SPACE_CHAR && *request!='\t' && *request!='\r' && *request!='\n' && *request!='\0')
				{
					port+=*request;
					++request;
				}
			}
			break;
		}
		++request;
	}

	if(host.length()>0 && port.length()==0)
	{
		while(SPACE_CHAR!=*urlStart)
		{
			if(':'==*urlStart)
			{
				++urlStart;
				while(SPACE_CHAR!=*urlStart && '/'!=*urlStart && '\\'!=*urlStart)
					port+=*urlStart++;
				break;
			}
			++urlStart;
		}

		if(port.length()==0)
			port="80";

		return;
	}

}

bool isConnectionAlive(const char* buffer,int len)
{
	int count=0;
	while(count<len && *buffer!='\0')
	{
		if(0==strncmp(buffer,"\nConnection:",strlen("\nConnection:")))
		{
			buffer+=strlen("\nConnection:")+1;
			while(SPACE_CHAR==*buffer)
				++buffer;

			if( 0==strncmp(buffer,"Close",strlen("Close") ))
				return false;

			return true;
		}
		++buffer;
	}

	return true;
}

DWORD Receive(SOCKET& requestSock,WSABUF& proxyRecvBuf,WSAOVERLAPPED& proxyRecvOverlapped,DWORD& Flag,DWORD timeOut,bool wait)
{
		DWORD nRead=0;

		int ret=WSARecv(requestSock,&proxyRecvBuf,1,&nRead,&Flag,&proxyRecvOverlapped,NULL);

		if(ret==SOCKET_ERROR && (WSA_IO_PENDING!=WSAGetLastError()))
		{
			cout<<"Error when receiving"<<endl;
			return SOCKET_ERROR;
		}
		ret=WSAWaitForMultipleEvents(1,&proxyRecvOverlapped.hEvent,TRUE,timeOut,TRUE);

		if(WAIT_FAILED==ret)
		{
			cout<<"wait failed"<<endl;
			return SOCKET_ERROR;
		}

		ret=WSAGetOverlappedResult(requestSock,&proxyRecvOverlapped,&nRead,wait,&Flag);
/*
		if(FALSE==ret)
		{
			cout<<"receiving from overlapped fail"<<endl;
			return SOCKET_ERROR;
		}
*/
		return nRead;
}

string GetTargetResponse(int threadid,const char* TargetURL, const char* Port, const char* request,SOCKET& output)
{
	SOCKET requestSock;
	SOCKADDR_IN clientAddr;
	struct hostent *pHost;	
	bool keepAlive=false;

	requestSock=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	pHost=gethostbyname(TargetURL);

	memset(&clientAddr,0,sizeof(clientAddr));

	int addrLen=sizeof(clientAddr);
	
	clientAddr.sin_family=AF_INET;
	memcpy(&clientAddr.sin_addr,pHost->h_addr, pHost->h_length);
	clientAddr.sin_port=htons(atoi(Port));

	if(0!=connect(requestSock,(SOCKADDR *)&clientAddr, sizeof(SOCKADDR_IN)))
	{
		closesocket(requestSock);
		return "";
	}
	
	WSAOVERLAPPED SendOverlapped;
	DWORD Flag=0;
	DWORD nSend=0;
	WSABUF clientRequestBuf;
	WSAEVENT sendEvent[1];

	clientRequestBuf.buf=(char*)request;
	clientRequestBuf.len=strlen(request);
	
	sendEvent[0]=WSACreateEvent();
	SendOverlapped.hEvent=sendEvent[0];

	int ret=WSASend(requestSock,&clientRequestBuf,1,&nSend,Flag,&SendOverlapped,NULL);

		if(ret==SOCKET_ERROR && (WSA_IO_PENDING!=WSAGetLastError()))
		{
			cout<<"Error when receiving"<<endl;
			return "";
		}

	DWORD nRead=0;
	const int bufferSize=RESPONSE_BUFFER_SIZE;
	char buf[bufferSize];
	char sendBuf[bufferSize];
	memset(buf,0,bufferSize);
	memset(sendBuf,0,bufferSize);
	string message="";

	WSAOVERLAPPED proxyResponseOverlapped,proxyRecvOverlapped;
	WSABUF proxyResponseBuf, proxyRecvBuf;
	WSAEVENT proxySendEvent[1],proxyRecvEvent[1];
	proxyResponseBuf.buf=sendBuf;
	proxyResponseBuf.len=bufferSize;
	proxyRecvBuf.buf=buf;
	proxyRecvBuf.len=bufferSize;


	proxySendEvent[0]=WSACreateEvent();
	proxyResponseOverlapped.hEvent=proxySendEvent[0];
	proxyRecvEvent[0]=WSACreateEvent();
	proxyRecvOverlapped.hEvent=proxyRecvEvent[0];
	
	int DATA_SIZE=bufferSize-1;
	int recvLen=0;

	::WSAWaitForMultipleEvents(1,sendEvent,FALSE,WSA_INFINITE,FALSE);
	
	ret=WSAGetOverlappedResult(requestSock,&SendOverlapped,&nSend,FALSE,&Flag);

//	nRead=Receive(requestSock,proxyRecvBuf,proxyRecvOverlapped,Flag,WSA_INFINITE,FALSE);
	nRead=recv(requestSock,proxyRecvBuf.buf,bufferSize,0);

	keepAlive=isConnectionAlive(proxyRecvBuf.buf,nRead);

	if(!keepAlive)
	{
			do
			{
				cout<<"Thread "<<threadid<<" [Connection:Closed]: "<<nRead<<endl;
					memcpy(sendBuf,buf,bufferSize);
//					WSASend(output,&proxyResponseBuf,1,&nSend,Flag,&proxyResponseOverlapped,NULL);
					send(output,sendBuf,nRead,0);
//					memset(buf,0,bufferSize);
					nRead=Receive(requestSock,proxyRecvBuf,proxyRecvOverlapped,Flag,WSA_INFINITE,FALSE);
//					nRead=recv(requestSock,buf,bufferSize,0);					
					if(0==nRead || SOCKET_ERROR==nRead)			
					{
						cout<<"socket error"<<endl;
						break;
					}

			}while(true);
	}
	else
	{
			do
			{
 				cout<<"Thread "<<threadid<<" [Connection:keep-alive]: "<<nRead<<endl;
					memcpy(sendBuf,buf,bufferSize);
//					WSASend(output,&proxyResponseBuf,1,&nSend,Flag,&proxyResponseOverlapped,NULL);
					send(output,sendBuf,nRead,0);

					memset(buf,0,bufferSize);
					nRead=Receive(requestSock,proxyRecvBuf,proxyRecvOverlapped,Flag,1000,FALSE);
					
					if(0==nRead || SOCKET_ERROR==nRead)			
					{
						cout<<"socket error"<<endl;
						break;
					}

			}while(true);
	}

		cout<<"close socket"<<endl;
		closesocket(requestSock);

		return "";

}

void RunServer()
{
	WSAData wsaData;
	if(WSAStartup(MAKEWORD(2,0),&wsaData)!=0)
	{
		cout<<"init failed"<<endl;
		return;
	}

	WSABUF dataBuf;
	WSAOVERLAPPED serverOverlapped;

	SOCKET serverSock, ioSock;
	SOCKADDR_IN serverAddr, clientAddr;
	memset(&serverAddr,0,sizeof(serverAddr));
	memset(&clientAddr,0,sizeof(clientAddr));

	int addrLen=sizeof(clientAddr);
	
//	InitSockAddr(serverAddr,AF_INET,htonl(INADDR_ANY),6666);
//	InitSockAddr(serverAddr,AF_INET,inet_addr("127.0.0.1"),6666);

//	serverSock=socket(AF_INET,SOCK_STREAM,0);
	serverSock=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	 if (serverSock == INVALID_SOCKET) 
     {
		   cout<<"invalid socket"<<endl;
           return;
      }

	 serverAddr.sin_family=AF_INET;
	 serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	 serverAddr.sin_port=htons(PROXY_PORT);
	
	if(SOCKET_ERROR==bind(serverSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr)))
	{
		cout<<"cannot bind to socket:"<<endl;
		return;
	}
	
	if(listen(serverSock,5))
	{
		cout<<"listen failed"<<endl;
		return;
	}

	char localHostName[255];
	hostent *local_ent=NULL;
	in_addr local_addr;
	string hostIP="";
	if(gethostname(localHostName,sizeof(localHostName))==SOCKET_ERROR)
	{
		memset(localHostName,0,sizeof(localHostName));
	}
	else
	{
			local_ent=gethostbyname(localHostName);
			int i=0;
			while(local_ent->h_addr_list[i])
			{
				if(i>0)
				{
					hostIP+='.';
				}
				memcpy(&local_addr,local_ent->h_addr_list[i],sizeof(local_addr));
				hostIP+=inet_ntoa(local_addr);
				++i;
			}
	}

	cout<<"start http proxy server at "<<hostIP<<":"<<PROXY_PORT<<endl;

	queueMutex=CreateMutex(NULL,FALSE,NULL);
//	threadSymaphore=CreateSemaphore(NULL,50,50,NULL);
	fd_set fdread;
	int ret;
	const int ThreadNumber=MAX_THREADS;
	int threadArg[ThreadNumber];
	HANDLE threads[ThreadNumber];
	unsigned int threadId[ThreadNumber];

	for(int i=0;i<ThreadNumber;++i)
	{
		threadArg[i]=i;
		threads[i]=(HANDLE)_beginthreadex(NULL,0,RequestThread,&threadArg[i],0,&threadId[i]);
	}

	while(true)
	{
//		ioSock=accept(serverSock,(sockaddr*) &serverAddr,&addrLen);
		ioSock=accept(serverSock,NULL,NULL);
		
			WaitForSingleObject(queueMutex,INFINITE);
			socketBuffer.push(ioSock);
			ReleaseMutex(queueMutex);

//			WaitForSingleObject(threadSymaphore,INFINITE);
//			HANDLE thread=(HANDLE)_beginthreadex(NULL,0,RequestThread,&threadArg,0,&threadId);
//		    ::ReleaseSemaphore(threadSymaphore,1,NULL);
	//		RequestThread();
	}


	shutdown(serverSock,2);
	closesocket(serverSock);
	WSACleanup();
}

bool InternetSetProxy(LPCTSTR conn_name,LPCTSTR proxy_full_addr);

int main(int argc, char** argv)
{
	GetSystemInfo(&sysInfo);

	cout<<"Demo HTTP Proxy Server ver. 0.1.0.0"<<endl;
	cout<<"Developed by: Like Zhang"<<endl;

	InternetSetProxy(NULL , TEXT("127.0.0.1:6666"));

	RunServer();

	InternetSetProxy(NULL, NULL);
	
	return 0;
}

