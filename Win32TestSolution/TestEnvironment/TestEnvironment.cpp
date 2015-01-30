// TestEnvironment.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <shlobj.h>

using namespace std;

void TestExpandCustomEnv()
{
	LPCTSTR src = TEXT("a %en% er~");
	TCHAR	buf[2048];

	std::cout<<"========== test - custom enviroment string expand ===========\n";
	std::cout<<"source : \n";
	std::wcout<< src <<std::endl;
	std::cout<<"\tbefore add environment : ";
	ExpandEnvironmentStrings(src, buf, ARRAYSIZE(buf));
	std::wcout<<buf<<std::endl;

	std::cout<<"\twhen add environment : ";
	SetEnvironmentVariable(TEXT("en"), TEXT("����"));

	ExpandEnvironmentStrings(src, buf, ARRAYSIZE(buf));
	std::wcout<<buf<<std::endl;

	std::cout<<"\tafter clear environment : ";
	SetEnvironmentVariable(TEXT("en"), NULL);

	ExpandEnvironmentStrings(src, buf, ARRAYSIZE(buf));
	std::wcout<<buf<<std::endl;

	// ���������չ���Զ��廷������
}

void TestPathWithEnv()
{
	std::cout<<"=================== test - path include environment variable ================\n";

	// ����һЩAPI�Ƿ�֧�ְ�������������·��
	//LPCTSTR src = TEXT("%SystemRoot%\notepad.exe");
	LPCTSTR src = TEXT("%DevLibDir%\NOTEPAD.EXE");
	DWORD attr = GetFileAttributes(src);
	std::cout<< "GetFileAttributes " << (attr != INVALID_FILE_ATTRIBUTES)<<std::endl;

	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(src);
	std::cout<< "ILCreateFromPath " << (0 != pidl)<<std::endl;

	SHFILEINFO info;
	SHGetFileInfo(src, NULL, &info, sizeof(info), SHGFI_ICON);
	std::cout<< "SHGetFileInfo " << (0 != info.hIcon) <<std::endl;

	HICON icon = ExtractIcon(NULL, src, 0);
	std::cout<< "ExtractIcon " << (0 != icon) <<std::endl;

	int code = (int)ShellExecute(NULL, TEXT("open"), src, NULL, NULL, SW_SHOWNORMAL);
	std::cout<< "ShellExecute " << (32 < code) <<std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");
	
	TestPathWithEnv();

	getchar();
	return 0;
}

