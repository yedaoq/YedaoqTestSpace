#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int __cdecl main_iniautocreate(int argc, _TCHAR* argv[])
{
	WritePrivateProfileString(TEXT("SECION"), TEXT("KEY1"), TEXT("abc"), TEXT("C:\\1.ini"));
	
	HANDLE file = CreateFile(TEXT("C:\\2.ini"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW | OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE != file)
	{
		CloseHandle(file);
	}

	WritePrivateProfileString(TEXT("SECION"), TEXT("KEY1"), TEXT("abc"), TEXT("C:\\2.ini"));

	return 0; 
}