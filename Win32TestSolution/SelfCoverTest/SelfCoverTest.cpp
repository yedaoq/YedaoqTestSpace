// SelfCoverTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SelfCover.h"
#include "SelfCoverDll.h"

void _tmain(int argc, _TCHAR* argv[])
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(NULL, path, MAX_PATH);

	std::cout<<"=======================================\n-> �����Ը��ǲ���: 0 - EXE, 1 - DLL1, 2 - DLL2\n\n";

	while (TRUE)
	{
		std::cout<<"-> ��ѡ�������: ";
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


