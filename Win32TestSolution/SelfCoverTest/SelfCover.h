#include <iostream>
#include <string>
#include "AppUpgradeUtil.h"

void CoutModuleFileName(HMODULE module)
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(module, path, MAX_PATH);
	std::wcout<<path;
}

int SelfCover(HMODULE module, AppUpgradeUtil& util)
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(module, path, MAX_PATH);

	std::cout<<"-> 将被替换的模块: ";
	std::wcout<<path<<std::endl;

	std::wstring tmp = path;

	util.FileReplace(tmp.c_str(), (tmp + TEXT(".new")).c_str());
	//AppUpgradeUtil::FileReplace(tmp.c_str(), (tmp + TEXT(".new")).c_str());

	return 0;

	MoveFile(path, (tmp + TEXT(".bak")).c_str());

	//std::cout<<"1. 重命名已完成，模块: ";
	//CoutModuleFileName(module);
	//getchar();

	if(0 !=_taccess((tmp + TEXT(".new")).c_str(), 0))
	{
		MoveFile((tmp + TEXT(".bak")).c_str(), path);
	}
	else
	{
		MoveFile((tmp + TEXT(".new")).c_str(), path);
	}

	//std::cout<<"\n2. 替换已完成，按任意键退出...";
	//getchar();

	return 0;
}