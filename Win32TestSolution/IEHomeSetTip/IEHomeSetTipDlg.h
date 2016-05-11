
// IEHomeSetTipDlg.h : 头文件
//

#pragma once
#include "explorer1.h"


// CIEHomeSetTipDlg 对话框
class CIEHomeSetTipDlg : public CDialog
{
// 构造
public:
	CIEHomeSetTipDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IEHOMESETTIP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_WebBrowser;
	afx_msg void OnBnClickedOk();
};
