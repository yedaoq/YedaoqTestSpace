
// IconSize.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIconSizeApp:
// �йش����ʵ�֣������ IconSize.cpp
//

class CIconSizeApp : public CWinAppEx
{
public:
	CIconSizeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIconSizeApp theApp;