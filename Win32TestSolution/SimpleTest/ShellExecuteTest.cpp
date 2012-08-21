#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int __cdecl main_ShellExecute(int argc, _TCHAR* argv[])
{
	//ShellExecute(NULL, NULL, TEXT("C:\\windows\\notepad.exe C:\\1.txt"), NULL /*TEXT("C:\\1.txt")*/, NULL, SW_SHOWNORMAL);
	//ShellExecute(NULL, TEXT("open"), TEXT("C:\\windows\\notepad.exe"), TEXT("C:\\1.txt"), NULL, SW_SHOWNORMAL);

	SHELLEXECUTEINFO ShExecInfo={0}; 
	ShExecInfo.cbSize=sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask=SEE_MASK_NOCLOSEPROCESS; 
	ShExecInfo.hwnd=NULL; 
	//ShExecInfo.lpVerb=L"runas"; 
	ShExecInfo.lpFile=L"c:\\windows\\system32\\notepad.exe";
	ShExecInfo.lpParameters=TEXT("C:\\1.txt");
	ShExecInfo.lpDirectory=NULL; 
	ShExecInfo.nShow=SW_SHOW; 
	ShExecInfo.hInstApp=NULL;
	ShellExecuteEx(&ShExecInfo); 
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE); 

	PROCESS_INFORMATION info;
	STARTUPINFO si = {0};
	si.cb = sizeof(STARTUPINFO);
	//CreateProcess(TEXT("C:\\windows\\notepad.exe"), TEXT(" C:\\1.txt"), 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(TEXT("C:\\windows\\notepad.exe"), TEXT("C:\\1.txt"), 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(TEXT("C:\\windows\\notepad.exe"), NULL, 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(NULL, TEXT("C:\\windows\\notepad.exe"), 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(NULL, TEXT("C:\\windows\\notepad.exe C:\\1.txt"), 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(NULL, TEXT("\"C:\\windows\\notepad.exe\" -h"), 0, 0, 0, 0, 0, 0, &si, &info);
	//CreateProcess(NULL, TEXT("\"C:\\windows\\notepad.exe\" C:\\1.txt"), 0, 0, 0, 0, 0, 0, &si, &info);//
	getchar();
	return 0; 
}