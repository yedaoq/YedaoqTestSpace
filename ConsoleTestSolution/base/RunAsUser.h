
#ifndef RunAsUser_h__
#define RunAsUser_h__

BOOL	IsUserAdmin();
HANDLE	GetDesktopProcess(DWORD dwDesiredAccess);
HANDLE	GetAccessTokenOfCurrentUserLogon();
HANDLE	GetAccessTokenWithoutAdminPrivilege();
bool	StartupProcessWithAcessToken(LPCTSTR cmd, HANDLE token);
bool	RestartCurrentProcessAsUser();
bool	RestartCurrentProcessWithoutAdminPrivilege();

#endif // RunAsUser_h__
