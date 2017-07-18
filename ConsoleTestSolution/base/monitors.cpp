#include "stdafx.h"
#include <Windows.h>

struct DeviceFlagInfo
{
	DWORD	flag;
	LPCTSTR desc;
};

// see wingdi.h
DeviceFlagInfo device_flags[] =
{
	{DISPLAY_DEVICE_ATTACHED_TO_DESKTOP, TEXT("Desktop") },
	{DISPLAY_DEVICE_MULTI_DRIVER, TEXT("MultiDriver") },
	{DISPLAY_DEVICE_PRIMARY_DEVICE, TEXT("Primary") },
	{DISPLAY_DEVICE_MIRRORING_DRIVER, TEXT("Mirror") },
	{DISPLAY_DEVICE_VGA_COMPATIBLE, TEXT("VGA") },
	{DISPLAY_DEVICE_REMOVABLE, TEXT("Removeable") },
	{DISPLAY_DEVICE_MODESPRUNED, TEXT("ModesPruned") },
	{DISPLAY_DEVICE_REMOTE, TEXT("Remote") },
	{DISPLAY_DEVICE_TS_COMPATIBLE, TEXT("Compatible") },
	{DISPLAY_DEVICE_DISCONNECT, TEXT("Disconnect") },
};


int _tmain_monitors(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	DISPLAY_DEVICE device;
	device.cb = sizeof(device);

	int idx = 0;
	while(true)
	{
		if (!EnumDisplayDevices(NULL, idx++, &device, 0))
			break;
		
		_tprintf(TEXT("device %d : \r\n\tID   - %s \r\n\tName - %s \r\n\tKey  - %s \r\n\tAdapter  - %s\r\n\tFlag - %08x"), idx, device.DeviceID, device.DeviceName, device.DeviceKey, device.DeviceString, device.StateFlags);
		
		for (int i = 0; i < ARRAYSIZE(device_flags); ++i)
		{
			if (device.StateFlags & device_flags[i].flag)
			{
				_tprintf(TEXT(" %s"), device_flags[i].desc);
			}
		}

		TCHAR device_name[33];
		_tcscpy(device_name, device.DeviceName);

		if (EnumDisplayDevices(device_name, 0, &device, 0))
		{
			_tprintf(TEXT("\r\n\tMonitor - %s"), device.DeviceName);
		}

		if (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
		{
			HDC dc = CreateDC(NULL, device_name, NULL, NULL);
			int driver_ver = GetDeviceCaps(dc, DRIVERVERSION);
			SIZE logic_size;
			logic_size.cx = GetDeviceCaps(dc, HORZRES);
			logic_size.cy = GetDeviceCaps(dc, VERTRES);
			int refresh_rate = GetDeviceCaps(dc, VREFRESH);
			_tprintf(TEXT("(%d, %d*%d, %dHz)"), driver_ver, logic_size.cx, logic_size.cy, refresh_rate);
			DeleteDC(dc);
		}

		_tprintf(TEXT("\r\n\r\n"));


		EnumMonitors(NULL, 2, )
			 
	}


	getchar();
	return 0;
}