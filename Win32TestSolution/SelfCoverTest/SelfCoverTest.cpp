// SelfCoverTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SelfCover.h"
#include "SelfCoverDll.h"

void _tmain(int argc, _TCHAR* argv[])
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(NULL, path, MAX_PATH);

	std::cout<<"=======================================\n-> 程序自覆盖测试: 0 - EXE, 1 - DLL1, 2 - DLL2\n\n";

	while (TRUE)
	{
		std::cout<<"-> 请选择测试项: ";
		int i;
		std::cin>>i;
		if(0 == i)
		{
			SelfCover(0);
		}
		else if(1 == i)
		{
			SelfCoverDll();
		}
		else
			break;
	}
}


