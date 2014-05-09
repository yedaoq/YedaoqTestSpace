#include "stdafx.h"
#include <atlbase.h>
#include <atlcom.h>
#include <oleacc.h>
#include <strsafe.h>

HANDLE	thread_handle_accget_ = NULL;
DWORD	thread_id_accget_ = 0;

bool StartAccGetThread();
void AsyncGetValue(HWND hWnd);
void ThreadProc(void* param);
static UINT GetObjectValue(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf);

bool StartAccGetThread()
{
	if(NULL == thread_handle_accget_ || INVALID_HANDLE_VALUE == thread_handle_accget_)
	{
		thread_handle_accget_ = (HANDLE)_beginthread(ThreadProc, 0, 0);
	}
	return NULL != thread_handle_accget_ && INVALID_HANDLE_VALUE != thread_handle_accget_;
}

void ThreadProc( void* param )
{
	CoInitialize(NULL);
	thread_id_accget_ = GetCurrentThreadId();

	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TCHAR buf[256];
		CComPtr<IAccessible> acc;
		if (S_OK == AccessibleObjectFromWindow((HWND)msg.lParam, OBJID_WINDOW, IID_IAccessible, (void**)&acc))
		{
			VARIANT id;
			id.vt = VT_I4;
			id.lVal = 0;
			GetObjectValue(acc, &id, buf, ARRAYSIZE(buf));
			OutputDebugString(TEXT("WinRun Text : "));
			OutputDebugString(buf);
		}
	}
	CoUninitialize();
}

static UINT GetObjectValue(IAccessible* pacc, VARIANT* pvarChild, LPWSTR lpszBuf, UINT cchBuf)
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

void AsyncGetValue( HWND hWnd )
{
	OutputDebugStringA("AsyncGetValue\n");
	PostThreadMessage(thread_id_accget_, WM_USER, 0, (LPARAM)hWnd);
}
