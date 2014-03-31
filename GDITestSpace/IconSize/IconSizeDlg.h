
// IconSizeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ImageUtil.h"


// CIconSizeDlg �Ի���
class CIconSizeDlg : public CDialog
{
// ����
public:
	CIconSizeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ICONSIZE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void ResetDraw();

	void SetStaticImageType(HWND hWndStatic, DWORD style);

	void DrawIcon(HICON hIcon);
	void DrawIcon(HBITMAP hBmp);

// ʵ��
protected:
	HICON m_hIcon;
	int	  m_DrawTargetID;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void OnBnClickedExtcopy();

	ImageUtil::GDIPlusInitialize gdiplus;
};

bool IPicture2File( IPicture* pic, LPCTSTR path );
bool IPicture2IStream( IPicture* pic, IStream** stream, LONG* size);
bool IStream2File( IStream* stream, LPCTSTR path );