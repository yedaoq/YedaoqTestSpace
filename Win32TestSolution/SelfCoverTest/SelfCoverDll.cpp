// SelfCoverDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "SelfCoverDll.h"
#include "SelfCover.h"

HMODULE dll_handle = 0;

// ���ǵ���������һ��ʾ��
SELFCOVERDLL_API int nSelfCoverDll=0;

// ���ǵ���������һ��ʾ����
SELFCOVERDLL_API int SelfCoverDll(void)
{
	/*return SelfCover(dll_handle);*/
	return 0;
}

