
// IEHomeSetTipDlg.h : ͷ�ļ�
//

#pragma once
#include "explorer1.h"


// CIEHomeSetTipDlg �Ի���
class CIEHomeSetTipDlg : public CDialog
{
// ����
public:
	CIEHomeSetTipDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IEHOMESETTIP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_WebBrowser;
	afx_msg void OnBnClickedOk();
};
