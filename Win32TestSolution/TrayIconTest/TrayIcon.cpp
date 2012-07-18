#include "StdAfx.h"
#include "TrayIcon.h"

using namespace nsYedaoqTrayIcon;

#define CHECKDELAYCREATE if(!DelayCreate()) return;

class CTrayIconHostWindow : ATL::CWindowImpl<CTrayIconHostWindow>
{
public:
	bool RegisterTrayIcon(__in ITrayIconCallBack* callback, __out HWND& host_wnd, __out UINT id);
	void UnRegisterTrayIcon(HWND wnd, UINT id);
};

nsYedaoqTrayIcon::CMorphingIcon::CMorphingIcon( DWORD _miniseconds_per_icon, WORD _cycle_times, WORD _icon_count, ... )
	: miniseconds_per_icon(_miniseconds_per_icon), cycle_times(_cycle_times), icon_count(_icon_count), icon_cur(_icon_count)
{
	ATLASSERT(icon_count > 0);
	icons = new HICON[icon_count];

	HICON* valist = (HICON*)(&_icon_count + 1);
	memcpy(icons, valist, sizeof(HICON) * icon_count);
}

nsYedaoqTrayIcon::CMorphingIcon::~CMorphingIcon()
{
	HICON* tmp = icons;
	if(icons)
	{
		icons = 0;
		for(int i = 0; i < icon_count; ++i)
		{
			DeleteObject(tmp[i]);
		}
		delete tmp;
	}
}

CTrayIcon::CTrayIcon( HWND hCallbackWnd, UINT uCallbackMessage, UINT uID, HICON hIcon, LPCTSTR lpszTip )
{
	memset(&data_, 0, sizeof(NOTIFYICONDATA));
	data_.hWnd = hCallbackWnd;
	data_.uCallbackMessage = uCallbackMessage;
	data_.uID = uID;
	data_.hIcon = hIcon;

	if(lpszTip && lpszTip[0])
	{
		_tcscpy_s(data_.szTip, lpszTip);
	}
}


CTrayIcon::~CTrayIcon(void)
{
}


void nsYedaoqTrayIcon::CTrayIcon::SetTip( LPCTSTR lpszTip )
{
	CHECKDELAYCREATE;

	if(lpszTip && lpszTip[0])
	{
		_tcscpy_s(data_.szTip, lpszTip);
	}
	else
	{
		data_.szTip[0] = 0;
	}

	data_.uFlags = NIF_TIP;
	Shell_NotifyIcon(NIM_MODIFY, &data_);
}

void nsYedaoqTrayIcon::CTrayIcon::SetIcon( HICON icon )
{
	CHECKDELAYCREATE;
	data_.uFlags = NIF_ICON;
	data_.hIcon = icon;
	Shell_NotifyIcon(NIM_MODIFY, &data_);
}

void nsYedaoqTrayIcon::CTrayIcon::SetIcon( CMorphingIcon* icon )
{

}

void nsYedaoqTrayIcon::CTrayIcon::Show( bool fShow )
{
	CHECKDELAYCREATE;
}

void nsYedaoqTrayIcon::CTrayIcon::ShowBalloonInfo( LPCTSTR lpszInfo, LPCTSTR lpszTitle, EnumBalloonIcon system_icon, HICON custom_icon, EnumBalloonFlag flag )
{
	if(lpszInfo && lpszInfo[0])
	{
		_tcscpy_s(data_.szInfo, lpszInfo);
	}
	else
	{
		data_.szInfo[0] = 0;
	}

	if(lpszTitle && lpszTitle[0])
	{
		_tcscpy_s(data_.szInfoTitle, lpszTitle);
	}
	else
	{
		data_.szInfoTitle[0] = 0;
	}

	HICON tmp = data_.hIcon;

	if(NULL != custom_icon)
	{
		data_.dwInfoFlags = NIIF_USER;
		data_.hIcon = custom_icon;
	}
	data_.dwInfoFlags |= flag;
	data_.uFlags = NIF_INFO;

	BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &data_);
	data_.hIcon = tmp;
}

bool nsYedaoqTrayIcon::CTrayIcon::DelayCreate()
{
	if(0xFFFF == data_.cbSize) return false;
	if(data_.cbSize > 0) return true;

	//Make sure Notification window is valid
	do 
	{
		data_.cbSize = 0xFFFF;

		if(!::IsWindow(data_.hWnd)) break;

		data_.cbSize = 0xFFFF;

	} while (false);

	if(0xFFFF == data_.cbSize)
	{
		return false;
	}

	// load up the NOTIFYICONDATA structure
	data_.cbSize = sizeof(NOTIFYICONDATA);
	data_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	// Set the tray icon
	BOOL bRet = Shell_NotifyIcon(NIM_ADD, &data_);
	if(!bRet)
	{
		data_.cbSize = 0;
	}
	return bRet;
}
