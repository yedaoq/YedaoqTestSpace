// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

HANDLE	pipe_handle_ = INVALID_HANDLE_VALUE;
LPCTSTR pipe_name_ = TEXT("\\\\.\\pipe\\yedaoqtest\\npipe");

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_ListLog.Attach(GetDlgItem(IDC_LISTLOG));

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	pipe_handle_ = CreateNamedPipe(pipe_name_,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		3,
		4086,
		4086,
		5000,
		NULL);

	if(pipe_handle_ != INVALID_HANDLE_VALUE)
	{
		m_ListLog.AddString(TEXT("wait for connect..."));
	}
	else{
		DWORD dwErr = GetLastError();
		m_ListLog.AddString(TEXT("error when create pipe!"));
	}

	if(ConnectNamedPipe(pipe_handle_, 0))
	{
		m_ListLog.AddString(TEXT("client connected!"));
	}
	else{
		m_ListLog.AddString(TEXT("error for client connect!"));
	}

	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


LRESULT CMainDlg::OnBnClickedBtnclient(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	while (1) 
	{ 
		pipe_handle_ = CreateFileW(pipe_name_,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION |
			/*FILE_FLAG_OVERLAPPED*/0,
			NULL);

		// Break if the pipe handle is valid. 
		if (pipe_handle_ != INVALID_HANDLE_VALUE) 
			break; 

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			m_ListLog.AddString(TEXT("Could not open pipe : pipe has been connected by other client\n")); 
			break;
		}

		// All pipe instances are busy, so wait for 20 seconds. 
		if (!WaitNamedPipe(pipe_name_, 20000) ) 
		{
			m_ListLog.AddString(TEXT("Could not wait pipe\n")); 
			break;
		}

	}

	if(pipe_handle_ != INVALID_HANDLE_VALUE)
	{
		m_ListLog.AddString(TEXT("connect to server!"));
	}

	//CallNamedPipe(TEXT("\\\\.\\yedaoqtest\npipe"), )
	return 0;
}
