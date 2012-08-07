// SelfCoverTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SelfCover.h"
#include "SelfCoverDll.h"
#include "AppUpgradeUtil.h"

void _tmain(int argc, _TCHAR* argv[])
{
	TCHAR buf[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, buf);
	::PathAppend(buf, TEXT("update.xml"));

	std::cout<<"=======================================\n-> 程序自覆盖测试: 0 - EXE, 1 - DLL1, 2 - Clear\n\n";

	while (TRUE)
	{
		std::cout<<"-> 请选择测试项: ";
		int i;
		std::cin>>i;
		if(0 == i)
		{
			SelfCover(0, buf);
		}
		else if(1 == i)
		{
			SelfCover(dll_handle, buf);
		}
		else if(2 == i)
		{
			AppUpgradeUtil::ClearObsoleteFile(buf);
		}
		else
			break;
	}
}


