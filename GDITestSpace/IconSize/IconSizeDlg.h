
// IconSizeDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

class GDIPlusInitialize
{
	ULONG_PTR     gdiplusToken_;
public:

	GDIPlusInitialize();
	~GDIPlusInitialize();
};

// CIconSizeDlg 对话框
class CIconSizeDlg : public CDialog
{
// 构造
public:
	CIconSizeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ICONSIZE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void ResetDraw();

	void SetStaticImageType(HWND hWndStatic, DWORD style);

	void DrawIcon(HICON hIcon);
	void DrawIcon(HBITMAP hBmp);

// 实现
protected:
	HICON m_hIcon;
	int	  m_DrawTargetID;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedLoadicon();
	afx_msg void OnBnClickedExtract();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedExtractex();
	afx_msg void OnBnClickedSetsm();
	afx_msg void OnBnClickedLoadimage();
	afx_msg void OnBnClickedIextract();
	afx_msg void OnBnClickedIextractimage();
	afx_msg void OnBnClickedShget();
	afx_msg void OnBnClickedShdefextract();


	DECLARE_MESSAGE_MAP()
public:
	CString m_FilePath;
	int		m_IconIdx;
	int		m_IconSize;
	int		m_IconCount;
	BOOL	m_FuLoad_DefaultSize;
	BOOL	m_FuLoad_MONOCHROME;
	BOOL	m_FuLoad_VGAColor;
	
	int m_CxIcon;
	int m_CxSmallIcon;

	CEdit m_MsgError;

	GDIPlusInitialize gdi;
	afx_msg void OnBnClickedGdipluscopy();
};
