
// IconSizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IconSize.h"
#include "IconSizeDlg.h"
#include <Strsafe.h>

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
	ON_BN_CLICKED(IDC_LoadImage, &CIconSizeDlg::OnBnClickedLoadimage)
	ON_BN_CLICKED(IDC_IExtract, &CIconSizeDlg::OnBnClickedIextract)
	ON_BN_CLICKED(IDC_IEXTRACTIMAGE, &CIconSizeDlg::OnBnClickedIextractimage)
	ON_BN_CLICKED(IDC_SHGET, &CIconSizeDlg::OnBnClickedShget)
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
	HWND hWndStatic = ::GetDlgItem(m_hWnd, m_DrawTargetID++);
	SetStaticImageType(hWndStatic, SS_ICON);
	::SendMessage(hWndStatic, STM_SETICON, (WPARAM) hIcon, NULL);
}

void CIconSizeDlg::DrawIcon( HBITMAP hBmp )
{
	HWND hWndStatic = ::GetDlgItem(m_hWnd, m_DrawTargetID++);
	SetStaticImageType(hWndStatic, SS_BITMAP);
	::SendMessage(hWndStatic, STM_SETIMAGE, (WPARAM) hBmp, NULL);
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

void CIconSizeDlg::OnBnClickedLoadimage()
{
	UpdateData(TRUE);

	ResetDraw();

	HMODULE module = NULL;
	if (!m_FilePath.IsEmpty())
	{
		module = LoadLibrary(m_FilePath);
	}

	DWORD fuLoad = 0;
	if(m_FuLoad_VGAColor) fuLoad |= LR_VGACOLOR;
	if(m_FuLoad_DefaultSize) fuLoad |= LR_DEFAULTSIZE;
	if(m_FuLoad_MONOCHROME) fuLoad |= LR_MONOCHROME;
	if(!module) fuLoad |= LR_LOADFROMFILE;

	HICON hIcon = (HICON)LoadImage(module, module ? MAKEINTRESOURCE(m_IconIdx) : (LPCTSTR)m_FilePath, IMAGE_ICON, m_IconSize, m_IconSize, fuLoad);

	if(hIcon)
		DrawIcon(hIcon);
	else
		m_MsgError.SetWindowText(TEXT("LoadImage fail"));
}

void CIconSizeDlg::OnBnClickedIextract()
{
	UpdateData(TRUE);

	ResetDraw();

	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(m_FilePath);
	if(NULL == pidl)
	{
		m_MsgError.SetWindowText(TEXT("invalid path"));
		return;
	}

	ATL::CComPtr<IShellFolder> shell_parent;
	PCIDLIST_RELATIVE pidl_child;

	HRESULT hr = SHBindToParent(pidl, IID_IShellFolder, (VOID**)&shell_parent, &pidl_child);
	if(FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get item folder info"));
		return;
	}

	ATL::CComPtr<IExtractIcon> icon_extractor;

	hr = shell_parent->GetUIObjectOf(NULL, 1, &pidl_child, IID_IExtractIcon, NULL, (VOID**)&icon_extractor);
	if(FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get item ui object"));
		return;
	}

	TCHAR	buf[1024];
	int		idx = 0;
	UINT   flag = 0;

	hr = icon_extractor->GetIconLocation(/*GIL_DEFAULTICON |*/ GIL_FORSHELL, buf, ARRAYSIZE(buf), &idx, &flag);
	if (FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get item icon location"));
		return;
	}

	HICON hIcon = NULL;

	TCHAR tmp[1024];
	StringCbPrintf(tmp, ARRAYSIZE(tmp), TEXT("%s,%d"), buf, idx);
	m_MsgError.SetWindowText(tmp);

	hr = icon_extractor->Extract(buf, idx, &hIcon, NULL, m_IconSize);
	if(SUCCEEDED(hr))
	{		
		DrawIcon(hIcon);
	}
}

void CIconSizeDlg::OnBnClickedIextractimage()
{
	UpdateData(TRUE);

	ResetDraw();

	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(m_FilePath);
	if(NULL == pidl)
	{
		m_MsgError.SetWindowText(TEXT("invalid path"));
		return;
	}

	ATL::CComPtr<IShellFolder> shell_parent;
	PCIDLIST_RELATIVE pidl_child;

	HRESULT hr = SHBindToParent(pidl, IID_IShellFolder, (VOID**)&shell_parent, &pidl_child);
	if(FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get item folder info"));
		return;
	}

	ATL::CComPtr<IExtractImage> image_extracter;

	hr = shell_parent->GetUIObjectOf(NULL, 1, &pidl_child, IID_IExtractImage, NULL, (VOID**)&image_extracter);
	if(FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get item image object"));
		return;
	}

	SIZE size = { m_IconSize, m_IconSize };
	DWORD dwPriority = 0, dwFlags = IEIFLAG_ASPECT;
	WCHAR pszImagePath[MAX_PATH + 1];
	hr = image_extracter->GetLocation(pszImagePath, MAX_PATH, &dwPriority, &size, 32, &dwFlags); 
	if (SUCCEEDED(hr))
	{
		HBITMAP hThumbnail;
		hr = image_extracter->Extract(&hThumbnail); 

		if(SUCCEEDED(hr))
		{
			DrawIcon(hThumbnail);
		}
		else
		{
			m_MsgError.SetWindowText(TEXT("can not get item image "));
		}
	}
	{
		m_MsgError.SetWindowText(TEXT("can not get item image location"));
	}
}

void CIconSizeDlg::SetStaticImageType( HWND hWndStatic, DWORD image_style )
{
	DWORD style = GetWindowLong(hWndStatic, GWL_STYLE);
	style &= ~(SS_BITMAP | SS_ICON);
	style |= image_style;

	SetWindowLong(hWndStatic, GWL_STYLE, style);
}

void CIconSizeDlg::OnBnClickedShget()
{
	UpdateData(TRUE);

	ResetDraw();

	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(m_FilePath);
	if(NULL == pidl)
	{
		m_MsgError.SetWindowText(TEXT("invalid path"));
		return;
	}

	SHFILEINFO info;
	BOOL ret = (BOOL)SHGetFileInfo((LPCTSTR)pidl, NULL, &info, sizeof(info), SHGFI_ICONLOCATION | SHGFI_PIDL);
	
	TCHAR tmp[1024];
	StringCbPrintf(tmp, ARRAYSIZE(tmp), TEXT("%s,%d"), info.szDisplayName, info.iIcon);
	m_MsgError.SetWindowText(tmp);

	ret = (BOOL)SHGetFileInfo((LPCTSTR)pidl, NULL, &info, sizeof(info), SHGFI_ICON | SHGFI_LARGEICON | SHGFI_PIDL);

	if(info.hIcon)
		DrawIcon(info.hIcon);

	ret = (BOOL)SHGetFileInfo((LPCTSTR)pidl, NULL, &info, sizeof(info), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_PIDL);

	if(info.hIcon)
		DrawIcon(info.hIcon);
	
}
