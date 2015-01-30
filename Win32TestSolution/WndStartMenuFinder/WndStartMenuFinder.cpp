// WndStartMenuFinder.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	MONITORINFO monitor_info = { sizeof(monitor_info) };
	HMONITOR	monitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(monitor, &monitor_info);

	POINT pt = { monitor_info.rcWork.left + 10, monitor_info.rcWork.bottom - 10 };
	TCHAR buf[1024];
	while(true)
	{
		HWND hwnd = WindowFromPoint(pt);
		hwnd = GetAncestor(hwnd, GA_ROOT);
		GetClassName(hwnd, buf, ARRAYSIZE(buf));
		_tprintf(buf);
		printf("\t\t");
		GetWindowText(hwnd, buf, ARRAYSIZE(buf));
		_tprintf(buf);
		printf("\n");
		if(_kbhit())
			break;

		Sleep(1000);
	}

	return 0;
}

