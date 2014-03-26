// CRuntime.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

void TestTtoi(LPCTSTR input)
{
	_tprintf(TEXT("Input  : \"%s\"\n"), input);

	int result = _ttoi(input);
	int err = errno;
	printf("Value : %d\n", result);
	printf("Error : %d\n\n", err);
}

int _tmain_ttoi(int argc, _TCHAR* argv[])
{
	LPCTSTR inputs[] = 
	{
		TEXT("31"),
		TEXT("-31"),

		TEXT("0x31"),
		TEXT("0xFF"),
		TEXT("FF"),
		
		TEXT("31.1"),
		TEXT("-31.1"),

		TEXT(" 31"),
		TEXT("\t31"),
		TEXT(" 31 "),
		TEXT(" 31\t"),
		TEXT(" 31 1"),
		TEXT("31\t 1"),
		TEXT("31 d "),

		TEXT("1ad "),
		TEXT("a1"),

		TEXT("99999999999999999"),
	};

	for (int i = 0; i < ARRAYSIZE(inputs); ++i)
	{
		TestTtoi(inputs[i]);
	}

	getchar();
	return 0;
}

