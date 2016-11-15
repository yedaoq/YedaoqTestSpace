// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "PipeDriver.h"

class MyPipeMessageHandler : public IPipeMessageHandler
{
public:
	MyPipeMessageHandler(WTL::CListBox* list_log)
		: list_log_(list_log)
	{}

	virtual void OnConnectFail()
	{
		list_log_->AddString(TEXT("连接失败！"));
	}

	virtual void OnConnectSucceed()
	{
		list_log_->AddString(TEXT("连接成功！"));
	}

	virtual void OnDisconnect()
	{
		list_log_->AddString(TEXT("连接断开！"));
	}

	virtual void OnMessgeArrived(LPCTSTR msg)
	{
		list_log_->AddString(msg);
	}

	virtual void OnRecvBufExtended()
	{
		list_log_->AddString(TEXT("缓冲区扩展！"));
	}

protected:
	WTL::CListBox* list_log_;
};

CPipeDriver* pipe_driver_ = 0;
MyPipeMessageHandler* pipe_handler_ = 0;

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

	m_EditMsg.Attach(GetDlgItem(IDC_EDTMSG));
	m_ListLog.Attach(GetDlgItem(IDC_LISTLOG));

	m_EditMsg.SetWindowText(TEXT("在此输入消息..."));

	pipe_driver_ = new CPipeDriver;
	pipe_handler_ = new MyPipeMessageHandler(&m_ListLog);


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
	pipe_driver_->Start(TEXT("yedaoqtest\20160624"), true, pipe_handler_);
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

LRESULT CMainDlg::OnBnClickedBtnclient( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	pipe_driver_->Start(TEXT("yedaoqtest\20160624"), false, pipe_handler_);
	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnSend( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	TCHAR buf[1024];
	m_EditMsg.GetWindowText(buf, ARRAYSIZE(buf) - 1);

	pipe_driver_->Send(buf);
	return 0;
}
