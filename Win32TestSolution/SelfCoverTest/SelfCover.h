#include <iostream>
#include <string>
#include "AppUpgradeUtil.h"

void CoutModuleFileName(HMODULE module)
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(module, path, MAX_PATH);
	std::wcout<<path;
}

int SelfCover(HMODULE module, LPCTSTR log_file)
{
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(module, path, MAX_PATH);

	std::cout<<"-> �����滻��ģ��: ";
	std::wcout<<path<<std::endl;

	std::wstring tmp = path;

	AppUpgradeUtil::FileReplace(path, (tmp + TEXT(".new")).c_str(), log_file);

	return 0;

	MoveFile(path, (tmp + TEXT(".bak")).c_str());

	//std::cout<<"1. ����������ɣ�ģ��: ";
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

	//std::cout<<"\n2. �滻����ɣ���������˳�...";
	//getchar();

	return 0;
}