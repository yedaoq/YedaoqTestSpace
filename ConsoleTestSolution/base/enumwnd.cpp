#include "stdafx.h"
#include <Windows.h>

static BOOL CALLBACK enum_windows_region_proc(__in  HWND hwnd, __in  LPARAM lParam)
{
	TCHAR title_buf[256];
	GetWindowText(hwnd, title_buf, ARRAYSIZE(title_buf));
	if (_tcscmp(title_buf, TEXT("Default IME")) && _tcscmp(title_buf, TEXT("MSCTFIME UI")) && 0 != *title_buf)
	{
		_tprintf(TEXT("%p %s\r\n"), hwnd, title_buf);
	}
	return TRUE;
}

int _main_enumwnd(int argc, _TCHAR* argv[])
{
// 	while(getchar())
// 	{
// 		EnumWindows(enum_windows_region_proc, 0);
// 		_tprintf(TEXT("\r\n\r\n"));
// 	}

	while(true)
	{
		HWND hwnd = GetForegroundWindow();
		TCHAR title_buf[256];
		GetWindowText(hwnd, title_buf, ARRAYSIZE(title_buf));
		_tprintf(TEXT("%p %s\r\n"), hwnd, title_buf);
		Sleep(1000);
	}

	return 0;
}