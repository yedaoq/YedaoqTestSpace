#include "stdafx.h"
#include <Windows.h>
#include <locale.h>

int _main_appbar(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	APPBARDATA apBar; 
	memset(&apBar,0,sizeof(apBar)); 
	apBar.cbSize = sizeof(apBar); 
	apBar.hWnd = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	if(apBar.hWnd != NULL) 
	{ 
		apBar.lParam = ABS_AUTOHIDE; 
		SHAppBarMessage(ABM_SETSTATE,&apBar); //设置任务栏自动隐藏
	}   

	if(apBar.hWnd != NULL) 
	{ 
		apBar.lParam = ABS_AUTOHIDE; 
		SHAppBarMessage(ABM_GETTASKBARPOS, &apBar); //设置任务栏自动隐藏
		_tprintf(TEXT("pos : %d"), apBar.uEdge);
	}  

	getchar();

	return 0;
}