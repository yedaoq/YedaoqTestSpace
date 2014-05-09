// TestIconOverlay.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TestIconOverlay.h"
#include <fstream>
#include <iosfwd>
#include <locale.h>
#include <strsafe.h>
#include <oleacc.h>
#include <atlcomcli.h>
#include <richedit.h>

extern bool StartAccGetThread();
extern void AsyncGetValue(HWND hWnd);

typedef LRESULT (CALLBACK *pWindowProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static UINT_PTR		timer_id_hook_ = NULL;
static pWindowProc	proc_prev_ = NULL;

VOID CALLBACK TimerProc_Hook(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK TimerProc_Hook1( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
LRESULT CALLBACK WindowProc_InputHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CTestIconOverlay::CTestIconOverlay()
	: m_cRef(1)
{}

STDMETHODIMP CTestIconOverlay::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
	if(ppv == 0)
		return E_POINTER;

	*ppv = NULL;

	if (IsEqualIID(riid, IID_IShellExtInit) || IsEqualIID(riid, IID_IUnknown))
	{
		OutputDebugStringA("CTestIconOverlay::QueryInterface IID_IShellExtInit | IID_IUnknown\n");
		*ppv = static_cast<LPSHELLEXTINIT>(this);
	}
	else if (IsEqualIID(riid, IID_IShellIconOverlayIdentifier))
	{
		OutputDebugStringA("CTestIconOverlay::QueryInterface IID_IShellIconOverlayIdentifier\n");
		*ppv = static_cast<IShellIconOverlayIdentifier*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CTestIconOverlay::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CTestIconOverlay::Release()
{
	if (--m_cRef)
		return m_cRef;

	delete this;
	return 0L;
}

STDMETHODIMP CTestIconOverlay::Initialize( __in_opt PCIDLIST_ABSOLUTE pidlFolder, __in_opt IDataObject *pdtobj, __in_opt HKEY hkeyProgID )
{
	//OutputDebugStringA("CTestIconOverlay::Initialize\n");
	return S_OK;
}

STDMETHODIMP CTestIconOverlay::IsMemberOf( LPCWSTR pwszPath, DWORD dwAttrib )
{
	//OutputDebugStringA("CTestIconOverlay::IsMemberOf\n");
	return S_FALSE;
}

STDMETHODIMP CTestIconOverlay::GetPriority( int *pPriority )
{
	OutputDebugStringA("CTestIconOverlay::GetPriority\n");
	*pPriority = 100;
	return S_OK;
}

STDMETHODIMP CTestIconOverlay::GetOverlayInfo( LPWSTR pwszIconFile, int cchMax, int *pIndex, DWORD *pdwFlags )
{
	StartAccGetThread();
	OutputDebugStringA("CTestIconOverlay::GetOverlayInfo\n");
	timer_id_hook_ = SetTimer(NULL, 0, 2000, TimerProc_Hook);
	return E_FAIL;
}

UINT GetObjectValue(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	LPWSTR  buf = 0;
	HRESULT hr = pacc->get_accValue(*pvarChild, &buf);
	if(buf)
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), buf);
		SysFreeString(buf);
	}
	else
	{
		DWORD err = GetLastError();
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

VOID CALLBACK TimerProc_Hook( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	//CoInitialize(NULL);

	//StartAccGetThread();

	HWND hwnd_startmenu = FindWindow(TEXT("DV2ControlHost"), NULL);
	HWND hwnd_input = NULL;
	if (hwnd_startmenu && 0 == proc_prev_)
	{
		HWND hwnd_tmp = FindWindowEx(hwnd_startmenu, NULL, TEXT("Desktop OpenBox Host"), NULL);
		if(hwnd_tmp)
		{
			hwnd_tmp = FindWindowEx(hwnd_tmp, NULL, TEXT("Search Box"), NULL);
			if(hwnd_tmp)
			{
				hwnd_tmp = FindWindowEx(hwnd_tmp, NULL, TEXT("SearchEditBoxWrapperClass"), NULL);
				if(hwnd_tmp)
				{
					hwnd_input = FindWindowEx(hwnd_tmp, NULL, TEXT("DirectUIHWND"), NULL);
				}
// 				if(hwnd_tmp)
// 				{
// 					hwnd_tmp = FindWindowEx(hwnd_tmp, NULL, TEXT("SearchBox"), NULL);
// 					if(hwnd_tmp)
// 					{
// 						hwnd_input = FindWindowEx(hwnd_tmp, NULL, TEXT("SearchEditBox"), NULL);
// 					}
// 				}

			}
		}
	}
	
	if(hwnd_input)
	{
		OutputDebugStringA("Window message Hooked\n");

// 		TCHAR buf[256];
// 		CComPtr<IAccessible> acc;
// 		if (S_OK == AccessibleObjectFromWindow(hwnd_input, OBJID_WINDOW, IID_IAccessible, (void**)&acc))
// 		{
// 			VARIANT id;
// 			id.vt = VT_I4;
// 			id.lVal = 0;
// 			GetObjectValue(acc, &id, buf, ARRAYSIZE(buf));
// 			OutputDebugString(TEXT("WinRun Text : "));
// 			OutputDebugString(buf);
// 		}
		//CoInitialize(NULL);
		proc_prev_ = (pWindowProc)SetWindowLongPtr(hwnd_input, GWLP_WNDPROC, (LONG_PTR)WindowProc_InputHook);
	}
	//CoUninitialize();

}

VOID CALLBACK TimerProc_Hook1( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	HWND hwnd_run = FindWindow(TEXT("#32770"), TEXT("运行"));
	HWND hwnd_input = NULL;
	if (hwnd_run && 0 == proc_prev_)
	{
		HWND hwnd_tmp = FindWindowEx(hwnd_run, NULL, TEXT("ComboBox"), NULL);
		if(hwnd_tmp)
		{
			hwnd_input = FindWindowEx(hwnd_tmp, NULL, TEXT("Edit"), NULL);
		}
	}

	if(hwnd_input)
	{
		OutputDebugStringA("run box Hooked\n");
		proc_prev_ = (pWindowProc)SetWindowLongPtr(hwnd_input, GWLP_WNDPROC, (LONG_PTR)WindowProc_InputHook);
	}
}

// UI元素的状态也表示成整型形式。因为一个状态可以有多个值，
//例如可选的、可做焦点的，该整数是反映这些值的位的或操作结果。
//将这些或数转换成相应的用逗号分割的状态字符串。
UINT GetObjectState(IAccessible* pacc,
					VARIANT* pvarChild,
					LPTSTR lpszState,
					UINT cchState)
{
	HRESULT hr;
	VARIANT varRetVal;

	*lpszState = 0;

	VariantInit(&varRetVal);

	hr = pacc->get_accState(*pvarChild, &varRetVal);

	if (!SUCCEEDED(hr))
		return(0);

	DWORD dwStateBit;
	int cChars = 0;
	if (varRetVal.vt == VT_I4)
	{
		// 根据返回的状态值生成以逗号连接的字符串。
		for (dwStateBit = STATE_SYSTEM_UNAVAILABLE;
			dwStateBit < STATE_SYSTEM_ALERT_HIGH;
			dwStateBit <<= 1)
		{
			if (varRetVal.lVal & dwStateBit)
			{
				cChars += GetStateText(dwStateBit,
					lpszState + cChars,
					cchState - cChars);
				*(lpszState + cChars++) = ',';
			}
		}
		if(cChars > 1)
			*(lpszState + cChars - 1) = '\0';
	}
// 	else if (varRetVal.vt == VT_BSTR)
// 	{
// 		WideCharToMultiByte(CP_ACP,
// 			0,
// 			varRetVal.bstrVal,
// 			-1,
// 			lpszState,
// 			cchState,
// 			NULL,
// 			NULL);
// 	}

	VariantClear(&varRetVal);

	return(lstrlen(lpszState));
}

UINT GetObjectName(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	LPWSTR buf = 0;
	pacc->get_accName(*pvarChild, &buf);
	if(buf)
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), buf);
		SysFreeString(buf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

UINT GetObjectClass(IAccessible* pacc, HWND* hWnd, LPWSTR lpszBuf, UINT cchBuf)
{
	*hWnd = NULL;
	WindowFromAccessibleObject(pacc, hWnd);
	if(*hWnd)
	{
		GetClassName(*hWnd, lpszBuf, cchBuf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

// UINT GetObjectRole(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
// {
// 	LPWSTR buf = 0;
// 	pacc->get_accRole(*pvarChild, &buf);
// 	if(buf)
// 	{
// 		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), buf);
// 		SysFreeString(buf);
// 	}
// 	return 0;
// }


UINT GetObjectValueEM(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	HWND hWnd = NULL;
	WindowFromAccessibleObject(pacc, &hWnd);
	if(hWnd)
	{
		GETTEXTEX gex = {0};
		gex.cb = cchBuf;
		gex.codepage = 1200;
		gex.flags = GT_DEFAULT;
		SendMessage(hWnd, EM_GETTEXTEX, (WPARAM)&gex, (LPARAM)lpszBuf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

UINT GetObjectValueEx(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	HWND hWnd = NULL;
	WindowFromAccessibleObject(pacc, &hWnd);
	if(hWnd)
	{
		GETTEXTEX gex = {0};
		gex.cb = cchBuf;
		gex.codepage = 1200;
		gex.flags = GT_DEFAULT;
		SendMessage(hWnd, EM_GETTEXTEX, (WPARAM)&gex, (LPARAM)lpszBuf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

UINT GetObjectDescription(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	LPWSTR buf = 0;
	pacc->get_accDescription(*pvarChild, &buf);
	if(buf)
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), buf);
		SysFreeString(buf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

UINT GetObjectRoleText(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	VARIANT role;
	role.vt = VT_I4;
	HRESULT hr = pacc->get_accRole(*pvarChild, &role);
	if(SUCCEEDED(hr))
	{
		GetRoleText(role.lVal, lpszBuf, cchBuf);
	}
	else
	{
		StringCbCopy(lpszBuf, cchBuf * sizeof(TCHAR), TEXT("Fail"));
	}
	return 0;
}

UINT AccGetChild(IAccessible* pacc, VARIANT& child_id, IAccessible** ppAccChild)
{
	IDispatch* disp = NULL;
	HRESULT hr = E_FAIL;
	switch(child_id.vt)
	{
	case VT_I4:
		hr = pacc->get_accChild(child_id, &disp);
		break;
	case VT_DISPATCH:
		disp = child_id.pdispVal;
		break;
	default:
		ATLASSERT(FALSE);
		break;
	}
	
	if(disp)
	{
		disp->QueryInterface(IID_IAccessible, (void**)ppAccChild);
		disp->Release();
	}

	return 0;
}

typedef BOOL (*EnumAccessibleProc)(IAccessible* acc, int deepth);

void EnumAccessibleDeepth(IAccessible* acc, EnumAccessibleProc proc, int deepth);

void EnumAccessible(IAccessible* acc, EnumAccessibleProc proc)
{
	EnumAccessibleDeepth(acc, proc, 0);
}
BOOL MyEnumAccessibleProc(IAccessible* acc, int deepth);

LRESULT CALLBACK WindowProc_InputHook( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (WM_KEYUP == uMsg)
	{
		AsyncGetValue(hwnd);
		TCHAR buf[1024];
		GetWindowText(hwnd, buf, ARRAYSIZE(buf));
		if(!*buf)
		{
// 			CoInitialize(NULL);
// 			CComPtr<IAccessible> acc;
// 			if (S_OK == AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, IID_IAccessible, (void**)&acc))
// 			{
// 				VARIANT id;
// 				id.vt = VT_I4;
// 				id.lVal = 0;
// 				GetObjectValue(acc, &id, buf, ARRAYSIZE(buf));
// 			
// 				CComPtr<IDispatch> dispParent;
// 				acc->get_accParent(&dispParent);
// 				CComPtr<IAccessible> accParent;
// 				dispParent->QueryInterface(IID_IAccessible, (VOID**)&accParent);
// 
// 				if(dispParent)
// 					EnumAccessible(accParent, MyEnumAccessibleProc);
// 			}
// 			CoUninitialize();
		}
// 		OutputDebugStringA("WinRun Text : ");
// 		OutputDebugString(buf);
// 		OutputDebugStringA("\n");
	}
	return proc_prev_(hwnd, uMsg, wParam, lParam);
}

void EnumAccessibleDeepth( IAccessible* acc, EnumAccessibleProc proc, int deepth )
{
	if(!proc(acc, deepth))
		return;

	HRESULT hr;
	long numChildren;
	unsigned long numFetched;
	VARIANT varChild;
	int index;
	IEnumVARIANT* pEnum = NULL;

	varChild.vt = VT_I4;

	//得到父亲支持的IEnumVARIANT接口
	hr = acc->QueryInterface(IID_IEnumVARIANT, (PVOID*)&pEnum);

	if(pEnum)
	{
		pEnum->Reset();
		acc->get_accChildCount(&numChildren);
		for(index = 1; index <= numChildren; index++)
		{
			hr = pEnum->Next(1, &varChild, &numFetched);  
			if(FAILED(hr))
			{
				OutputDebugStringA("Fail to get a child from enumerator\n");
				continue;
			}

			IAccessible* pAccChild = NULL;
			AccGetChild(acc, varChild, &pAccChild);
			if(pAccChild)
			{
				EnumAccessibleDeepth(pAccChild, proc, deepth + 1);
				pAccChild->Release();
			}
		}
	}
}

BOOL MyEnumAccessibleProc( IAccessible* acc, int deepth )
{
	TCHAR szName[256],szClass[256],szValue[256],szDesc[256],szRole[256];
	HWND hWnd;

	VARIANT var;
	var.vt = VT_I4;
	var.lVal = 0;

	*szName = *szClass = *szValue = *szDesc = *szRole = 0;

	GetObjectName(acc, &var, szName, 256);
	GetObjectClass(acc, &hWnd, szClass, 256);
	GetObjectValue(acc, &var, szValue, 256);
	GetObjectDescription(acc, &var, szDesc, 256);
	GetObjectRoleText(acc, &var, szRole, 256);
	GetObjectValueEM(acc, &var, szValue, 256);

	TCHAR buf[1024];
	StringCbPrintf(buf, sizeof(buf), TEXT("Deepth:%d; \tName:%s; WndClass:%s; Value:%s; HWND:%p; Desc:%s; Role:%s"), deepth, szName, szClass, szValue, hWnd, szDesc, szRole);

	OutputDebugString(buf);
	return TRUE;
}
