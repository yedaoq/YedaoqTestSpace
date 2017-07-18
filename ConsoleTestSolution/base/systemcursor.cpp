#include "stdafx.h"
#include <Windows.h>
#include <locale.h>

int _main_systemcursor(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	TCHAR buf[1024];

	LPWSTR system_cursors[] = {IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_ICON, IDC_NO, IDC_SIZE, IDC_SIZEALL};
	LPCTSTR system_cursor_names[] = {TEXT("IDC_APPSTARTING"), TEXT("IDC_ARROW"), TEXT("IDC_CROSS"), TEXT("IDC_HAND"), TEXT("IDC_HELP"), TEXT("IDC_IBEAM"), TEXT("IDC_ICON"), TEXT("IDC_NO"), TEXT("IDC_SIZE"), TEXT("IDC_SIZEALL")};

	for (int i = 0; i < ARRAYSIZE(system_cursors); ++i)
	{
		_tprintf_s( TEXT("%s : 0x%p\r\n"), system_cursor_names[i], LoadCursor(NULL, system_cursors[i]));
	}

	CURSORINFO info;
	while(TRUE)
	{
		info.cbSize = sizeof(info);
		GetCursorInfo(&info);
		_tprintf_s(TEXT("Current Cursor : 0x%p (%04d,%04d)\r\n"), info.hCursor, info.ptScreenPos.x, info.ptScreenPos.y);
		Sleep(1000);
	}
	

	getchar();

	return 0;
}