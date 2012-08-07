// SelfCoverTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SelfCover.h"
#include "SelfCoverDll.h"
#include "AppUpgradeUtil.h"

AppUpgradeUtil g_uputil;

void _tmain(int argc, _TCHAR* argv[])
{
	TCHAR buf[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, buf);
	::PathAppend(buf, TEXT("update.xml"));
	g_uputil.SetLogFileName(buf);

	std::cout<<"=======================================\n-> 程序自覆盖测试: 0 - EXE, 1 - DLL1, 2 - Clear\n\n";

	while (TRUE)
	{
		std::cout<<"-> 请选择测试项: ";
		int i;
		std::cin>>i;
		if(0 == i)
		{
			SelfCover(0, g_uputil);
		}
		else if(1 == i)
		{
			SelfCover(dll_handle, g_uputil);
		}
		else if(2 == i)
		{
			//AppUpgradeUtil::ClearObsoleteFile(buf, true);
			g_uputil.ClearObsoleteFile();
		}
		else
			break;
	}

	g_uputil.SaveLogFile();
}


