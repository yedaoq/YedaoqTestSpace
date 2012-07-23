#ifndef YedaoqCppLib_Win32_TrayIcon_h__
#define YedaoqCppLib_Win32_TrayIcon_h__

#include <shellapi.h>

#ifndef NIIF_LARGE_ICON
#define NIIF_LARGE_ICON 0x00000020
#endif

namespace nsYedaoqTrayIcon
{
	interface ITrayIconCallBack;
	enum EnumBalloonIcon {TIBI_INFO = NIIF_INFO, TIBI_WARNING = NIIF_WARNING, TIBI_ERROR = NIIF_ERROR };
	enum EnumBalloonFlag {TIBF_NONE = 0, TIBF_NOSOUND = NIIF_NOSOUND, TIBF_LARGE_ICON = NIIF_LARGE_ICON };

	struct CMorphingIcon
	{
		CMorphingIcon(DWORD _miniseconds_per_icon, WORD _cycle_times, WORD _icon_count, ...);
		~CMorphingIcon();

		HICON	next() { ++icon_cur; if(icon_cur >= icon_count) icon_cur = 0; return icons[icon_cur]; }

		DWORD	miniseconds_per_icon;
		WORD	icon_cur;
		WORD	cycle_times;

	protected:		
		HICON*	icons;
		WORD	icon_count;
	};

	class CTrayIcon
	{
		struct TrayIconData
		{
			DWORD	cbSize;
			HWND	hWnd;
			UINT	uID;
			UINT	uFlags;
			UINT	uCallbackMessage;
			HICON	hIcon;
			TCHAR	szTip[128];
			DWORD	dwState;
			DWORD	dwStateMask;
			TCHAR	szInfo[256];
			union {
				UINT  uTimeout;
				UINT  uVersion;  // used with NIM_SETVERSION, values 0, 3 and 4
			} DUMMYUNIONNAME;
			TCHAR	szInfoTitle[64];
			DWORD	dwInfoFlags;
			GUID	guidItem;
			HICON	hBalloonIcon;
		};

	public:
		CTrayIcon(HWND hCallbackWnd, UINT uCallbackMessage, UINT uID, HICON hIcon, LPCTSTR lpszTip);
		~CTrayIcon(void);

		void SetTip(LPCTSTR lpszTip);
		void SetIcon(HICON icon);
		void SetIcon(CMorphingIcon* icon);
		
		void Show(bool fShow);
		void ShowBalloonInfo(LPCTSTR lpszInfo, LPCTSTR lpszTitle, EnumBalloonIcon system_icon, HICON custom_icon, EnumBalloonFlag flag);

	protected:
		bool DelayCreate();
		static DWORD GetAdaptiveSize4IconData();

	protected:
		NOTIFYICONDATA data_;
	};
}
#endif // YedaoqCppLib_Win32_TrayIcon_h__

