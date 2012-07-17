// SimpleShlExt.cpp : CSimpleShlExt 的实现

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
		// 当在文件夹中空白位置点击右键时，pDataObj为空指针

		// 1. 根据pidlFolder查询文件夹路径的方法1
		SHGetPathFromIDList(pidlFolder, buf);

		// 2. 根据pidlFolder查询文件夹路径的方法2 , available above vista
		wchar_t* ret;
		SHGetNameFromIDList(pidlFolder, SIGDN_FILESYSPATH, &ret);		

		return S_OK;
	}

	// 在数据对象内查找 CF_HDROP 型数据. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}

	// 获得指向实际数据的指针
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// 检查非NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	// 有效性检查 – 保证最少有一个文件名. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	// 取得第一个文件名并把它保存在类成员m_szFile 中.
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