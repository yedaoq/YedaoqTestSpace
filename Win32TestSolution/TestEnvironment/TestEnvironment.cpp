// TestEnvironment.cpp : 定义控制台应用程序的入口点。
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
	SetEnvironmentVariable(TEXT("en"), TEXT("嗯嗯"));

	ExpandEnvironmentStrings(src, buf, ARRAYSIZE(buf));
	std::wcout<<buf<<std::endl;

	std::cout<<"\tafter clear environment : ";
	SetEnvironmentVariable(TEXT("en"), NULL);

	ExpandEnvironmentStrings(src, buf, ARRAYSIZE(buf));
	std::wcout<<buf<<std::endl;

	// 结果：可以展开自定义环境变量
}

void TestPathWithEnv()
{
	std::cout<<"=================== test - path include environment variable ================\n";

	// 测试一些API是否支持包含环境变量的路径
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

