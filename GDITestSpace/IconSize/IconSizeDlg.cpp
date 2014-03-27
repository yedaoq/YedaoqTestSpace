
// IconSizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IconSize.h"
#include "IconSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIconSizeDlg 对话框
CIconSizeDlg::CIconSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconSizeDlg::IDD, pParent)
	, m_FilePath(_T(""))
	, m_IconIdx(0)
	, m_IconSize(0)
	, m_IconCount(0)
	, m_FuLoad_DefaultSize(FALSE)
	, m_FuLoad_MONOCHROME(FALSE)
	, m_FuLoad_VGAColor(FALSE)
	, m_CxIcon(0)
	, m_CxSmallIcon(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIconSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDTFILE, m_FilePath);
	DDX_Text(pDX, IDC_EDTIDX, m_IconIdx);
	DDX_Text(pDX, IDC_EDTSIZE, m_IconSize);
	DDX_Text(pDX, IDC_EDTNUM, m_IconCount);
	DDX_Check(pDX, IDC_CHKSIZE, m_FuLoad_DefaultSize);
	DDX_Check(pDX, IDC_CHECK2, m_FuLoad_MONOCHROME);
	DDX_Check(pDX, IDC_CHKVGA, m_FuLoad_VGAColor);
	DDX_Text(pDX, IDC_EDIT1, m_CxIcon);
	DDX_Text(pDX, IDC_EDIT2, m_CxSmallIcon);
	DDX_Control(pDX, IDC_EDIT3, m_MsgError);
}

BEGIN_MESSAGE_MAP(CIconSizeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LoadIcon, &CIconSizeDlg::OnBnClickedLoadicon)
	ON_BN_CLICKED(IDC_Extract, &CIconSizeDlg::OnBnClickedExtract)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_ExtractEx, &CIconSizeDlg::OnBnClickedExtractex)
	ON_BN_CLICKED(IDC_SETSM, &CIconSizeDlg::OnBnClickedSetsm)
END_MESSAGE_MAP()


// CIconSizeDlg 消息处理程序

BOOL CIconSizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_CxIcon = GetSystemMetrics(SM_CXICON);
	m_CxSmallIcon = GetSystemMetrics(SM_CXSMICON);
	
	m_IconIdx = m_IconCount = m_IconSize = 0;

	m_FilePath = TEXT("C:\\Windows\\notepad.exe");

	UpdateData(FALSE);

	DragAcceptFiles(TRUE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIconSizeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIconSizeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIconSizeDlg::ResetDraw()
{
	m_MsgError.SetWindowText(TEXT(""));
	m_DrawTargetID = IDC_PIC0;

	for (int i = IDC_PIC0; i < IDC_PIC7; ++i)
	{
		::SendMessage(::GetDlgItem(m_hWnd, i), STM_SETICON, (WPARAM) NULL, NULL);
	}
}

void CIconSizeDlg::DrawIcon( HICON hIcon )
{
	::SendMessage(::GetDlgItem(m_hWnd, m_DrawTargetID++), STM_SETICON, (WPARAM) hIcon, NULL);
}


void CIconSizeDlg::OnBnClickedLoadicon()
{
	UpdateData(TRUE);

	ResetDraw();

	HMODULE module = AfxGetInstanceHandle();
	if (!m_FilePath.IsEmpty())
	{
		module = LoadLibrary(m_FilePath);
	}
	else
	{
		m_IconIdx = IDR_MAINFRAME;
	}

	if(module)
	{
		HICON hIcon = LoadIcon(module, MAKEINTRESOURCE(m_IconIdx));
		
		if(hIcon)
			DrawIcon(hIcon);
		else
			m_MsgError.SetWindowText(TEXT("LoadIcon fail"));

		if(module != AfxGetInstanceHandle())
			FreeLibrary(module);
	}
	else
		m_MsgError.SetWindowText(TEXT("LoadLibrary fail"));
}



void CIconSizeDlg::OnBnClickedExtract()
{
	UpdateData(TRUE);

	ResetDraw();

	if(!m_FilePath.IsEmpty())
	{
		HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), m_FilePath, m_IconIdx);
		if(hIcon)
			DrawIcon(hIcon);
		else
			m_MsgError.SetWindowText(TEXT("ExtractIcon fail"));
	}
	else
		m_MsgError.SetWindowText(TEXT("Please set file path"));
}

void CIconSizeDlg::OnDropFiles(HDROP hDropInfo)
{
	DragQueryFile(hDropInfo, 0, m_FilePath.GetBuffer(1024), 1024);
	m_FilePath.ReleaseBuffer();

	UpdateData(FALSE);
}

void CIconSizeDlg::OnBnClickedExtractex()
{
	UpdateData(TRUE);

	ResetDraw();

	if(m_IconCount < 1)
		m_IconCount = 1;

	UpdateData(FALSE);

	if(!m_FilePath.IsEmpty())
	{
		HICON icons[8] = {0};

		if(0 == ExtractIconEx(m_FilePath, m_IconIdx, icons, icons + 4, m_IconCount))
		{
			m_MsgError.SetWindowText(TEXT("ExtractIconEx fail"));
		}
		else
		{
			for (int i = 0; i < ARRAYSIZE(icons); ++i)
			{
				DrawIcon(icons[i]);
			}
		}
	}
	else
		m_MsgError.SetWindowText(TEXT("Please set file path"));
}

void CIconSizeDlg::OnBnClickedSetsm()
{

}
