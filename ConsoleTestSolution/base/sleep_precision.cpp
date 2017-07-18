#include "stdafx.h"
#include <Windows.h>
#include <locale.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <math.h>
#include <iostream>

#pragma comment(lib,"shlwapi.lib")

int _main_sleepprecision(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	TCHAR buf[1024];

	LARGE_INTEGER	qpf;
	QueryPerformanceFrequency(&qpf);
	printf("PerformanceFrequency : %i64\n", qpf.QuadPart);
	qpf.QuadPart = qpf.QuadPart / 1000;

	LARGE_INTEGER	qpc_begin, qpc_end;

	for (int i = 0; i < 100; ++i)
	{
		QueryPerformanceCounter(&qpc_begin);
		int sleep_interval = qpc_begin.LowPart % 50;
		if (sleep_interval < 3) sleep_interval = 3;
		
		Sleep(sleep_interval);
		QueryPerformanceCounter(&qpc_end);

		LONGLONG true_interval = (qpc_end.QuadPart - qpc_begin.QuadPart) / qpf.QuadPart;
		printf("sleep %02d %02d\r\n", sleep_interval, (int)true_interval);
		Sleep(10);
	}

	getchar();

	return 0;
}