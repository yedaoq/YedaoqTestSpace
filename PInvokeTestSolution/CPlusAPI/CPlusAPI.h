#ifndef CPlusAPI_h__
#define CPlusAPI_h__

enum {intType, doubleType, SockAddrType, charType, wcharType, charsType, wcharsType};

int GetDataInt(int* data_ptr);
int GetDataIntRef(int& data);

int GetDataDouble(double* data_ptr);
int GetDataDoubleRef(double& data);

int GetDataSockaddr(SOCKADDR_IN* data_ptr);
int GetDataSockaddrRef(SOCKADDR_IN& data);

int GetDataChar(char* data_ptr);
int GetDataCharRef(char& data_ptr);

int GetDataWchar(wchar_t* data_ptr);
int GetDataWcharRef(wchar_t& data_ptr);

int GetDataStr(char* data_ptr);
int GetDataStrEx(char (&data)[MAX_PATH]);

int GetDataWstr(wchar_t* data_ptr);
int GetDataWstrEx(wchar_t (&data)[MAX_PATH]);

#endif // CPlusAPI_h__
