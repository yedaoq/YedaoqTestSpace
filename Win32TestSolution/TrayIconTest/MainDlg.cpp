// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

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

	ctl_edit_tip_ = GetDlgItem(IDC_TXTTIP);

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
	CloseDialog(wID);
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

LRESULT CMainDlg::OnClose( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	if(EnumCloseAction::Unknow == close_action_)
	{
		int ret = MessageBox(TEXT("'YES' - 关闭应用程序\r\n'NO' - 最少化到系统托盘\r\n'Cancel' - 取消操作"), TEXT("请选择操作"), MB_YESNOCANCEL);
		if(ret == IDYES)
			close_action_ = Exit;
		else if(ret == IDNO)
			close_action_ = Hide;
		else
		{
			return 0;
		}
	}

	if(close_action_ == Exit)
	{
		bHandled = FALSE;
		return 0;
	}
	else
	{

	}

	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnhover(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	NOTIFYICONDATA d;
	memset(&d, 0, sizeof(d));
	d.cbSize = sizeof(NOTIFYICONDATA);
	d.dwInfoFlags = NIF_TIP;
	d.hWnd = m_hWnd;
	d.uID = 1;

	ctl_edit_tip_.GetWindowText(d.szTip, sizeof(d.szInfo) / sizeof(d.szInfo[0]));

	Shell_NotifyIcon(NIM_MODIFY, &d);

	return 0;
}


LRESULT CMainDlg::OnBnClickedBtntip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnquery(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码


	return 0;
}


LRESULT CMainDlg::OnBnClickedBtncreate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	NOTIFYICONDATA d;
	memset(&d, 0, sizeof(d));
	d.cbSize = sizeof(NOTIFYICONDATA);
	d.hWnd = m_hWnd;
	d.uID = 1;

	if(trayicon_id_>0)
	{
		d.dwInfoFlags = 0;
		trayicon_id_ = -1;
		ATLASSERT(Shell_NotifyIcon(NIM_DELETE, &d));
	}
	else
	{
		d.dwInfoFlags = NIF_TIP | NIF_MESSAGE | NIF_ICON;
		ctl_edit_tip_.GetWindowText(d.szTip, sizeof(d.szInfo) / sizeof(d.szInfo[0]));
		d.uCallbackMessage = WM_TRAYNOTIFY;
		//d.hIcon = (HICON)SendMessage(WM_GETICON, ICON_SMALL, 0);
		d.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		trayicon_id_ = 1;

		ATLASSERT(Shell_NotifyIcon(NIM_ADD, &d));
		DWORD dwError = ::GetLastError();
		if(dwError != S_OK)
		{
			ATLTRACE2(TEXT("abc"));
		}
	}



	return 0;
}
