// SelfCoverDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SelfCoverDll.h"

HMODULE dll_handle = 0;

// 这是导出变量的一个示例
SELFCOVERDLL_API int nSelfCoverDll=0;

// 这是导出函数的一个示例。
SELFCOVERDLL_API int SelfCoverDll(void)
{
	/*return SelfCover(dll_handle);*/
	return 0;
}

