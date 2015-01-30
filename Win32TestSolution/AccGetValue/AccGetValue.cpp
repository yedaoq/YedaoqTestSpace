// AccGetValue.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <OleAcc.h>
#include <strsafe.h>
#include <iostream>

#pragma comment(lib, "Oleacc.lib")

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

UINT GetObjectValueEx(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
{
	ATL::CComPtr<IDispatch>		disp_parent;
	ATL::CComPtr<IAccessible>	acc_parent;
	if(0 == pvarChild->lVal)
	{
		pacc->get_accParent(&disp_parent);
		disp_parent->QueryInterface(IID_IAccessible, (VOID**)&acc_parent);
		//pacc->
	}

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

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");
	CoInitialize(NULL);

	POINT cur_pos = {0, 0};
	POINT cur_pos_gotten = {0, 0};
	while(true)
	{
		POINT cur_pos_new;
		GetCursorPos(&cur_pos_new);
		if (cur_pos_new.x == cur_pos.x && cur_pos_new.y == cur_pos.y && (cur_pos_gotten.x != cur_pos.x && cur_pos_gotten.y != cur_pos.y))
		{
			cur_pos_gotten.x = cur_pos.x;
			cur_pos_gotten.y = cur_pos.y;

			ATL::CComPtr<IAccessible>	acc;
			VARIANT						child_id;
			child_id.vt = VT_I4;
			HRESULT hr = AccessibleObjectFromPoint(cur_pos, &acc, &child_id);
			if(SUCCEEDED(hr))
			{
				TCHAR szName[256],szClass[256],szValue[256],szDesc[256],szRole[256];
				HWND hWnd;

				*szName = *szClass = *szValue = *szDesc = *szRole = 0;

				GetObjectName(acc, &child_id, szName, 256);
				GetObjectClass(acc, &hWnd, szClass, 256);
				GetObjectValue(acc, &child_id, szValue, 256);
				GetObjectDescription(acc, &child_id, szDesc, 256);
				GetObjectRoleText(acc, &child_id, szRole, 256);

				TCHAR buf[1024];
				StringCbPrintf(buf, sizeof(buf), TEXT("Name:%s; WndClass:%s; Value:%s; HWND:%p; Desc:%s; Role:%s"), szName, szClass, szValue, hWnd, szDesc, szRole);

				std::wcout<<buf<<std::endl<<std::endl;
				
				VariantClear(&child_id);
			}
			else
			{
				printf("AccessibleObjectFromPoint Fail\n");
			}
		}
		else
		{
			cur_pos.x = cur_pos_new.x;
			cur_pos.y = cur_pos_new.y;
		}

		Sleep(500);
	}

	return 0;
}

