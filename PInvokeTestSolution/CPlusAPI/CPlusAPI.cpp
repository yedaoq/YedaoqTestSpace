#include <WinSock.h>
#include "CPlusAPI.h"

int GetDataInt( int* data_ptr )
{
	if(data_ptr) *data_ptr = 2;
}

int GetDataIntRef( int& data )
{
	data = 3;
}

int GetDataDouble( double* data_ptr )
{
	if(data_ptr) *data_ptr = 4.0;
}

int GetDataDoubleRef( double& data )
{
	data = 5.0;
}

int GetDataSockaddr( SOCKADDR_IN* data_ptr )
{
	if(data_ptr)
	{
		data_ptr->sin_addr.S_un.S_un_b.s_b1 = 192;
		data_ptr->sin_addr.S_un.S_un_b.s_b2 = 168;
		data_ptr->sin_addr.S_un.S_un_b.s_b3 = 0;
		data_ptr->sin_addr.S_un.S_un_b.s_b4 = 1;

		data_ptr->sin_port = 80;
		data_ptr->sin_family = AF_INET;
	}
}

int GetDataSockaddrRef( SOCKADDR_IN& data )
{
	data.sin_addr.S_un.S_un_b.s_b1 = 10;
	data.sin_addr.S_un.S_un_b.s_b2 = 1;
	data.sin_addr.S_un.S_un_b.s_b3 = 8;
	data.sin_addr.S_un.S_un_b.s_b4 = 110;

	data.sin_port = 8000;
	data.sin_family = AF_INET;
}

int GetDataChar( char* data_ptr )
{
	if(data_ptr) *data_ptr = 7;
}

int GetDataCharRef( char& data_ptr )
{
	data_ptr = 8;
}

int GetDataWchar( wchar_t* data_ptr )
{
	if(data_ptr) *data_ptr = 9;
}

int GetDataWcharRef( wchar_t& data )
{
	data = 10;
}

int GetDataStr( char* data_ptr )
{
	if(data_ptr) strcpy(data_ptr, "hello world!");
}

int GetDataStrEx( char (&data)[MAX_PATH] )
{
	strcpy(data, "hello earth!");
}

int GetDataWstr( wchar_t* data_ptr )
{
	if(data_ptr) wcscpy(data_ptr, L"hello coder!");
}

int GetDataWstrEx( wchar_t (&data)[MAX_PATH] )
{
	wcscpy(data, L"hello programmer!");
}