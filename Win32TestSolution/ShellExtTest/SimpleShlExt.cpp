// SimpleShlExt.cpp : CSimpleShlExt µÄÊµÏÖ

#include "stdafx.h"
#include "SimpleShlExt.h"
#include "CreateProcessHelper.h"

// CSimpleShlExt

HRESULT CSimpleShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )  
{
	//MessageBox(NULL, TEXT("test"), TEXT("test"), MB_OK);
	CCreateProcessHelper::CreateProcessDetached(TEXT("E:\\YedaoqTestSpace\\Win32TestSolution\\ResLoadTest\\Debug\\ResLoadTest.exe"), 0);
	return S_OK;
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