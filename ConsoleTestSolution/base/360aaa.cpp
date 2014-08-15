#include "stdafx.h"
#include <Windows.h>
#include <locale.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <iostream>

#pragma comment(lib,"shlwapi.lib")

int _main_360aaa(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	TCHAR buf[1024];

	while(true)
	{
		char c;
		printf("\n\nInvoke from (l - shortcut in start menu; e - exe in program files) : ");
		std::cin >> c;
		if('l' == c)
		{
			SHGetSpecialFolderPath(NULL, buf, CSIDL_PROGRAMS, FALSE);
			::PathAppend(buf, TEXT("Accessories\\Windows Time Plus\\日历时钟扩展.lnk"));
			if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(buf))
			{
				SHGetSpecialFolderPath(NULL, buf, CSIDL_PROGRAMS, FALSE);
				::PathAppend(buf, TEXT("附件\\Windows Time Plus\\日历时钟扩展.lnk"));
			}
		}
		else if('e' == c)
		{
			SHGetSpecialFolderPath(NULL, buf, CSIDL_PROGRAM_FILES, FALSE);
			::PathAppend(buf, TEXT("Windows Time Plus\\Calendar.exe"));
		}
		else if('\n' == c || '\r' == c)
		{
			printf("line char\n");
		}
		else
		{
			break;
		}

		_tprintf(buf);
		
		if(INVALID_FILE_ATTRIBUTES != GetFileAttributes(buf))
		{
			//int ret = (int)ShellExecute(NULL, NULL, TEXT("C:\\Windows\\System32\\cmd.exe"), buf, NULL, SW_SHOWNORMAL);
			int ret = (int)ShellExecute(NULL, NULL/*TEXT("open")*/, buf, NULL, NULL, SW_SHOWNORMAL);
			if(ret <= 32)
			{
				printf(" invoke fail : %d", ret);
			}
		}
		else
		{
			printf(" not found!\n", buf);
		}
	}

	return 0;
}