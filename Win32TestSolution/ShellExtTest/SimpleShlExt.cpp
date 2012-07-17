// SimpleShlExt.cpp : CSimpleShlExt ��ʵ��

#include "stdafx.h"
#include "SimpleShlExt.h"
#include "CreateProcessHelper.h"
#include <shlobj.h>

// CSimpleShlExt

HRESULT CSimpleShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )  
{
	//MessageBox(NULL, TEXT("test"), TEXT("test"), MB_OK);
	CCreateProcessHelper::CreateProcessDetached(TEXT("E:\\YedaoqTestSpace\\Win32TestSolution\\ResLoadTest\\Debug\\ResLoadTest.exe"), 0);
	
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
	STGMEDIUM stg = { TYMED_HGLOBAL };
	TCHAR	buf[MAX_PATH];
	HDROP hDrop; 

	if (!pDataObj)
	{
		// �����ļ����пհ�λ�õ���Ҽ�ʱ��pDataObjΪ��ָ��

		// 1. ����pidlFolder��ѯ�ļ���·���ķ���1
		SHGetPathFromIDList(pidlFolder, buf);

		// 2. ����pidlFolder��ѯ�ļ���·���ķ���2 , available above vista
		wchar_t* ret;
		SHGetNameFromIDList(pidlFolder, SIGDN_FILESYSPATH, &ret);		

		return S_OK;
	}

	// �����ݶ����ڲ��� CF_HDROP ������. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}

	// ���ָ��ʵ�����ݵ�ָ��
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// ����NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	// ��Ч�Լ�� �C ��֤������һ���ļ���. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	// ȡ�õ�һ���ļ������������������Աm_szFile ��.
	for (int i = 0; i < uNumFiles; ++i)
	{
		if ( 0 == DragQueryFile ( hDrop, i, buf, MAX_PATH ))
		{ 
			hr = E_INVALIDARG; 
			break;
		} 
	}
	
	GlobalUnlock ( stg.hGlobal ); 
	ReleaseStgMedium ( &stg ); 
	return hr; 
} 

HRESULT CSimpleShlExt::QueryContextMenu ( HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{
	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T("SimpleShlExt Test Item") ); 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}

HRESULT CSimpleShlExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo )
{
	MessageBox(NULL, TEXT("HAHA"), TEXT("tip"), MB_OK);
	return S_OK;
}

HRESULT CSimpleShlExt::GetCommandString(UINT idCmd, UINT uFlags,UINT* pwReserved, LPSTR pszName, UINT cchMax )
{
	return E_INVALIDARG; 
}