#include "stdafx.h"
#include <windows.h>
#include <intrin.h>
#include <winnt.h>
#include <smmintrin.h>

//#pragma comment(lib, "ntdll.lib")

typedef bool (*cmp_fun_t)(char*, char*, int );
typedef SIZE_T (CALLBACK *RtlCompareMemory_t)(CONST VOID *Source1, CONST VOID *Source2, SIZE_T Length);
RtlCompareMemory_t	rtlCompareMemory_;

bool cmp_sse(char* a, char* b, int len)
{
	int align = sizeof(__m128i) - (unsigned __int64)a % sizeof(__m128i);

	if (align < sizeof(__m128i))
	{
		for (int i = 0; i < align; ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}
	}

	//printf("cmp %p, %p %d\r\n", a, b, align);

	__m128i* a1 = (__m128i*)(a + align);
	__m128i* b1 = (__m128i*)(b + align);

	int len1 = len / (sizeof(*a1) / sizeof(*a));

	for (int i = 0; i < len1; ++i)
	{
		__m128i res = _mm_cmpeq_epi64( *a1++, *b1++);
		if (!res.m128i_i32[0] || !res.m128i_i32[2])
		{
			printf("err %p, %p   %p  %p  %p  %p\r\n", a1, b1, res.m128i_i32[0], res.m128i_i32[1], res.m128i_i32[2], res.m128i_i32[3]);
			return false;
		}
	}

	for (int i = (char*)a1 - a; i < len; ++i)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}

	return true;
}

bool cmp_rtl(char* a, char* b, int len)
{
	return len == rtlCompareMemory_(a, b, len);
	return false;
}

bool cmp_memcmp(char* a, char* b, int len)
{
	return 0 == memcmp(a, b, len);
}

bool cmp_int(char* a, char* b, int len)
{
	int* a1 = (int*)a;
	int* b1 = (int*)b;

	int l = len / (sizeof(*a1) / sizeof(*a) );

	for (int i = 0; i < l; ++i)
	{
		if (*a1++ != *b1++)
		{
			return false;
		}
	}

	for (int i = (char*)a1 - a; i < len; ++i)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}

	return true;
}

int _main_memcmp(int argc, _TCHAR* argv[])
{
	const int block_size = 1200 * sizeof(int);

	int* a = (int*)malloc(block_size);		// do not free to make sure compare different memory block
	int* b = (int*)malloc(block_size);

	for (int i = 0; i < block_size / sizeof(int); ++i)
	{
		a[i] = i;
	}

	memcpy(b, a, 1200 * sizeof(int));

	bool  equal_flag;

	DWORD tickcount_begin;
	DWORD tickcount_end;

	HMODULE ntdll_module = LoadLibrary(TEXT("ntdll.dll"));
	rtlCompareMemory_ = (RtlCompareMemory_t)GetProcAddress(ntdll_module, "RtlCompareMemory");

	cmp_fun_t cmp_funs[] = { cmp_int, cmp_memcmp, cmp_rtl, cmp_sse };
	char* cmp_fun_names[] = { "int", "memcmp", "RtlCompareMemory", "sse" };

	for (int i = 0; i < ARRAYSIZE(cmp_funs); ++i)
	{
		tickcount_begin = GetTickCount();
		for (int j = 0; j < 100000; ++j)
		{
			equal_flag = cmp_funs[i]((char*)a, (char*)b, block_size);
			if (!equal_flag)
			{
				printf("error!\r\n");
			}
		}
		tickcount_end = GetTickCount();

		printf("compare block by %s cost : %d\r\n", cmp_fun_names[i], tickcount_end - tickcount_begin);
	}

	getchar();

	return 0;
}
