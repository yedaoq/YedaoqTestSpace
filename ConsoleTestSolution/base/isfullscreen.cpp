#include "stdafx.h"
#include <Windows.h>

bool IsThereAnyAppFullScreen()
{
	HWND hwnd_foreground = GetForegroundWindow();
	HMONITOR monitor_handle = MonitorFromWindow(hwnd_foreground, MONITOR_DEFAULTTOPRIMARY);
	if(monitor_handle)
	{
		MONITORINFO monitor_info = { sizeof(monitor_info) };
		RECT rc;

		if ( GetMonitorInfo(monitor_handle, &monitor_info) 
			&& GetWindowRect(hwnd_foreground, &rc))
		{
			return monitor_info.rcMonitor.left == rc.left &&
				monitor_info.rcMonitor.right == rc.right &&
				monitor_info.rcMonitor.top == rc.top &&
				monitor_info.rcMonitor.bottom == rc.bottom;
		}
	}
	return false;
}

int _main_fullscreen(int argc, _TCHAR* argv[])
{
	while(true)
	{
		bool fullscreen = IsThereAnyAppFullScreen();
		printf("fullscreen : %s\n", fullscreen ? "yes" : "no");
		Sleep(1000);
	}
	return 0;
}