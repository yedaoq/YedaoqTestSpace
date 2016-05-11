// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "USN.h"

UsnRecordEnumerator* usn_enumerator;

void MyEnumUsnRecordProc(const USN_RECORD* record, LPVOID user_data)
{
	WTL::CListViewCtrl* ctl_usn = (WTL::CListViewCtrl*)user_data;
	int idx = ctl_usn->AddItem(ctl_usn->GetItemCount(), 0, record->FileName);
	//ctl_usn->SetItem(idx, 0, LVIF_TEXT, record->FileName, 0, 0, 0, 0);	
}

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

	ctl_find.Attach(GetDlgItem(IDC_FIND));
	ctl_usn.Attach(GetDlgItem(IDC_USN));

	ctl_usn.AddColumn(TEXT("Name"), 0);
	ctl_usn.AddColumn(TEXT("No."), 1);
	ctl_usn.AddColumn(TEXT("Parent No."), 2);
	ctl_usn.AddColumn(TEXT("Attribute"), 3);

	usn_enumerator = new UsnRecordEnumerator();
	usn_enumerator->Initialize('D');

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
	//EnumUsnRecord('D', MyEnumUsnRecordProc, &ctl_usn);

	TCHAR key_to_search[128];
	ctl_find.GetWindowText(key_to_search, ARRAYSIZE(key_to_search));

	PUSN_RECORD record;
	for(usn_enumerator->Reset(); usn_enumerator->MoveNext(&record); )
	{
		if(_tcsstr(record->FileName, key_to_search))
		{
			int idx = ctl_usn.AddItem(ctl_usn.GetItemCount(), 0, record->FileName);
		}		
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
