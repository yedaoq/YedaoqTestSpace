// whip.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "whip.h"
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <tchar.h>
#include <atlstr.h>

#include "base/at_exit.h"

#include "include/IRefObj.h"
#include "cpl/SystemUtil.h"
#include "HttpThread/HttpThread.h"

#pragma comment(lib, "base.lib")
#pragma comment(lib, "cpl.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "HttpThread.lib")

TCHAR exe_path_[2048] = TEXT("");

void AddToSys();
void Report();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (_tcsstr(lpCmdLine, TEXT("/ui")))
	{
		AddToSys();
	}
	else
	{
		TCHAR buf[1024];
		DWORD dwType, dwSize;
		LSTATUS ls = SHGetValue(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), TEXT("whip"), &dwType, buf, &dwSize);
		if (ERROR_FILE_NOT_FOUND == ls)
		{
			if(GetModuleFileName(NULL, exe_path_, ARRAYSIZE(exe_path_) - 1))
			{
				TCHAR os_ver[20] = TEXT("5");
				cpl::GetOSVersion(os_ver, ARRAYSIZE(os_ver));
				LPCTSTR action = (os_ver[0] > '5') ? TEXT("runas") : NULL;

				ShellExecute(NULL, action, exe_path_, TEXT("/ui"), NULL, SW_HIDE);
			}
		}
		else
		{
			Report();
		}
	}

	return 0;
}

void AddToSys()
{
	if(GetModuleFileName(NULL, exe_path_, ARRAYSIZE(exe_path_) - 1))
	{
		TCHAR target_path[2048];
		if(SHGetSpecialFolderPath(NULL, target_path, CSIDL_PROGRAM_FILESX86, FALSE))
		{
			PathAppend(target_path, TEXT("whip"));
			CreateDirectory(target_path, NULL);
			PathAppend(target_path, TEXT("whip.exe"));

			if (_tcscmp(target_path, exe_path_))
			{
				if(!CopyFile(exe_path_, target_path, FALSE))
					_tcscpy(target_path, exe_path_);

				SHSetValue(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), TEXT("whip"), REG_SZ, target_path, (_tcslen(target_path) + 1) * sizeof(TCHAR));
			}
		}
	}
}

void Report()
{
	base::AtExitManager at_exit_manager;
	std::locale prev_loc = std::locale::global(std::locale("chs"));

	auto_refptr<HttpThread> http_thread = new HttpThread("whip");
	http_thread->Startup();
	int sid = http_thread->Get(TEXT("http://wwipg.sinaapp.com?update=1"), NULL, NULL, NULL, NULL);
	http_thread->Shutdown(15000);
}


