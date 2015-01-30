#include "stdafx.h"
#include <WtsApi32.h>
#include <userenv.h>
#include "RunAsUser.h"

#pragma comment(lib, "WtsApi32.lib")

#define INVALID_SESSION_ID 0xFFFFFFFF

BOOL IsUserAdmin()  
{  
	BOOL                     bIsAdmin              = FALSE;  
	SID_IDENTIFIER_AUTHORITY NtAuthority           = SECURITY_NT_AUTHORITY;  
	PSID                     AdministratorsGroup;   

	if(AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,  
		0, 0, 0, 0, 0, 0, &AdministratorsGroup)) 
	{  
		if (!CheckTokenMembership( NULL, AdministratorsGroup, &bIsAdmin))   
		{  
			bIsAdmin = FALSE;  
		}   
		FreeSid(AdministratorsGroup);   
	}  

	return(bIsAdmin);  
}  

HANDLE GetDesktopProcess(DWORD dwDesiredAccess)
{
	//HWND	wnd_desktop = GetDesktopWindow();	// 这个窗口其实属于csrss.exe进程
	HWND	wnd_desktop = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	DWORD	process_id = 0;
	HANDLE	process_handle = NULL;
	GetWindowThreadProcessId(wnd_desktop, &process_id);

	if(NULL != wnd_desktop && 0 != process_id)
	{
		process_handle = OpenProcess( dwDesiredAccess /*PROCESS_QUERY_INFORMATION*/, FALSE, process_id);
	}

	return process_handle;
}

bool RestartCurrentProcessAsUser()
{
	HANDLE	desktop_process = GetDesktopProcess(PROCESS_QUERY_INFORMATION);
	HANDLE	hAccessToken; 
	bool	bRet = false;
	if(desktop_process && OpenProcessToken(desktop_process, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &hAccessToken))
	{
		TCHAR exe_path[2048];
		if(GetModuleFileName(NULL, exe_path, ARRAYSIZE(exe_path) - 1))
		{
			PROCESS_INFORMATION info;
			STARTUPINFO si = { sizeof(si) };    
			if(CreateProcessAsUser(hAccessToken, exe_path, NULL, NULL, NULL, FALSE, 0 /*NORMAL_PRIORITY_CLASS*/, NULL, NULL, &si, &info ))
			{
				CloseHandle(info.hProcess);
				CloseHandle(info.hThread);
				bRet = true;
			}
		}
		CloseHandle(hAccessToken);
	}
	return bRet;
}

// bool RestartCurrentProcessAsUser()
// {
// 	HANDLE	hAccessToken = GetAccessTokenOfCurrentUserLogon();
// 	LPVOID	lpEnvironment = NULL;
// 	bool  	bRet = false;
// 	if(hAccessToken && CreateEnvironmentBlock(&lpEnvironment, hAccessToken, FALSE))
// 	{
// 		TCHAR exe_path[2048] = TEXT("\"");
// 		if(GetModuleFileName(NULL, exe_path, ARRAYSIZE(exe_path) - 3))
// 		{
// 			PROCESS_INFORMATION info;
// 			STARTUPINFO si = { sizeof(si) };    
// 			si.dwFlags = STARTF_USESHOWWINDOW;   
// 
// 			if(CreateProcessAsUser(hAccessToken, exe_path, NULL, NULL, NULL, FALSE, 0 /*NORMAL_PRIORITY_CLASS*/, NULL, NULL, &si, &info ))
// 			{
// 				CloseHandle(info.hProcess);
// 				bRet = true;
// 			}
// 			else
// 			{
// 				OutputDebugStringA("CreateProcessAsUser fail!\n");
// 			}
// 		}
// 	}
// 
// 	if(lpEnvironment)
// 		DestroyEnvironmentBlock(lpEnvironment);
// 
// 	if(hAccessToken)
// 		CloseHandle(hAccessToken);
// 
// 	return bRet;
// }

HANDLE GetAccessTokenOfCurrentUserLogon()
{
	HANDLE hToken = NULL;
	HANDLE hTokenDuplicated = NULL;
	
	// 获得当前Session ID
	DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	if(INVALID_SESSION_ID != dwSessionID)
	{
		if (WTSQueryUserToken(dwSessionID, &hToken))
		{
			DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,	SecurityIdentification, TokenPrimary, &hTokenDuplicated);
			CloseHandle(hToken);
		}
	}

	return hTokenDuplicated;
}

bool RestartCurrentProcessWithoutAdminPrivilege()
{
	//HANDLE hAccessToken = GetAccessTokenOfCurrentUserLogon();
	HANDLE hAccessToken = GetAccessTokenWithoutAdminPrivilege();
	if(hAccessToken)
	{
		TCHAR exe_path[2048] = TEXT("\"");
		if(GetModuleFileName(NULL, exe_path, ARRAYSIZE(exe_path) - 1))
		{
			return StartupProcessWithAcessToken(exe_path, hAccessToken);
		}
	}

	return false;
}

HANDLE GetAccessTokenWithoutAdminPrivilege()
{
	HANDLE hToken,hNewToken = NULL;
	SID_AND_ATTRIBUTES SidAttrib;
	SID_IDENTIFIER_AUTHORITY ia= {SECURITY_NT_AUTHORITY}; // SECURITY_LOCAL_SID_AUTHORITY; //{{0,0,0,0,0,5}}; //SECURITY_NT_AUTHORITY
	LUID_AND_ATTRIBUTES Privileges[]={{{2}},{{3}},{{4}},{{5}},{{6}},{{7}},{{8}},{{9}},{{10}},{{11}},{{12}},{{13}},{{14}},{{15}},{{16}},{{17}},{{18}},{{20}},{{21}},{{22}},{{24}},{{26}},{{27}},{{28}},{{29}},{{30}}};

	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS /*| TOKEN_QUERY | TOKEN_DUPLICATE*/, &hToken))
	{
		if(AllocateAndInitializeSid(&ia, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &SidAttrib.Sid))
		{
			SidAttrib.Attributes = 0;
			CreateRestrictedToken(hToken, 0, 1, &SidAttrib, ARRAYSIZE(Privileges), Privileges, 0, 0, &hNewToken);
			FreeSid(SidAttrib.Sid);
		}
		CloseHandle(hToken);
	}

	return hNewToken;
}

// HANDLE GetAccessTokenWithoutAdminPrivilege()
// {
// 	HANDLE hToken;
// 	HANDLE hTokenDuplicated;
// 	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY | TOKEN_DUPLICATE, &hToken))
// 	{
// 		DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,	SecurityIdentification, TokenPrimary, &hTokenDuplicated);
// 		CloseHandle(hToken);
// 	}
// 
// 	if(hTokenDuplicated)
// 	{
// 		BYTE                     InfoBuffer[1024];  
// 		DWORD                    dwInfoBufferSize;  
// 		if(GetTokenInformation(hTokenDuplicated, TokenGroups, InfoBuffer, 1024, &dwInfoBufferSize))
// 		{
// 			PSID                     psidAdministrators;  
// 			SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;  
// 			if(AllocateAndInitializeSid(&siaNtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,  
// 				0,0,0,0,0,0, &psidAdministrators))  
// 			{
// 				PTOKEN_GROUPS ptgGroups = (PTOKEN_GROUPS)InfoBuffer;  
// 				for(UINT i = 0; i < ptgGroups->GroupCount; ++i)  
// 				{  
// 					if(EqualSid(psidAdministrators,ptgGroups->Groups[i].Sid))  
// 					{  
// 						bRet = true;  
// 						break;  
// 					}  
// 				}  
// 
// 				FreeSid(psidAdministrators);  
// 			}
// 		}
// 		CloseHandle( hAccessToken );  
// 	}
// 
// 	
// 	return hNewToken;
// }

bool StartupProcessWithAcessToken( LPCTSTR cmd, HANDLE token )
{
	PROCESS_INFORMATION info;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);    
	//si.dwFlags = STARTF_USESHOWWINDOW;   

	if(CreateProcessAsUser(token, cmd, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &info ))
	{
		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
		return true;
	}
// 	else
// 	{
// 		OutputDebugStringA("CreateProcessAsUser fail!\n");
// 	}

	return false;
}

bool MakesureReceiveDragDropMessagae()
{
	HMODULE module = LoadLibrary(TEXT("User32.dll"));
	if(module)
	{
		typedef BOOL (CALLBACK *pChangeWindowMessageFilter)(UINT, DWORD);
		pChangeWindowMessageFilter api_wmf = (pChangeWindowMessageFilter)GetProcAddress(module, "ChangeWindowMessageFilter");
		if(api_wmf)
		{
			api_wmf(0x49, MSGFLT_ADD);
			api_wmf(WM_DROPFILES, MSGFLT_ADD);
		}
		FreeLibrary(module);
	}
	return true;
}
