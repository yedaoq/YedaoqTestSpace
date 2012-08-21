#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int __cdecl main_delaydelete(int argc, _TCHAR* argv[])
{
	TCHAR module_path[MAX_PATH + 1];
	GetModuleFileName(NULL, module_path, MAX_PATH);

	if(!MoveFileEx(module_path, 0, 4))
	{
		MessageBox(NULL, TEXT("MoveFileEx Fail"), NULL, MB_OK);
	}

	return 0; 
}