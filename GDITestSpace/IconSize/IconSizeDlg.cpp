
// IconSizeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IconSize.h"
#include "IconSizeDlg.h"
#include <Strsafe.h>

#include <gdiplus.h>
#include <shlwapi.h>

#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL SaveIcon3(TCHAR *szIconFile, HICON hIcon[], int nNumIcons);

// CIconSizeDlg �Ի���
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
	ON_BN_CLICKED(IDC_SHDEFEXTRACT, &CIconSizeDlg::OnBnClickedShdefextract)
	ON_BN_CLICKED(IDC_GDIPLUSCOPY, &CIconSizeDlg::OnBnClickedGdipluscopy)
	ON_BN_CLICKED(IDC_IMAGEFACTORY, &CIconSizeDlg::OnBnClickedImagefactory)
END_MESSAGE_MAP()


// CIconSizeDlg ��Ϣ�������

BOOL CIconSizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_CxIcon = GetSystemMetrics(SM_CXICON);
	m_CxSmallIcon = GetSystemMetrics(SM_CXSMICON);
	
	m_IconIdx = m_IconCount = m_IconSize = 0;

	m_FilePath = TEXT("C:\\Windows\\notepad.exe");

	UpdateData(FALSE);

	DragAcceptFiles(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIconSizeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//SetStaticImageType(hWndStatic, SS_ICON);
	::SendMessage(hWndStatic, STM_SETICON, (WPARAM) hIcon, NULL);

	SaveIcon3(TEXT("D:\\test.ico"), &hIcon, 1);
}

HBITMAP CreateVritualIconMask(HBITMAP bmColor)
{
	BITMAP bitmap;
	if(sizeof(BITMAP) < GetObject(bmColor, sizeof(bitmap), &bitmap))
		return NULL;

	int row_width_in_bytes = ((bitmap.bmWidth + 31) / 32) * 4;
	LPBYTE bmData = new BYTE[row_width_in_bytes * bitmap.bmHeight];
	memset(bmData, 0, row_width_in_bytes * bitmap.bmHeight);

	HBITMAP bmp = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, bmData);
	return bmp;
}

void CIconSizeDlg::DrawIcon( HBITMAP hBmp )
{
	ICONINFO info;
	info.fIcon = 1;
	info.hbmColor = hBmp;
	info.hbmMask = CreateVritualIconMask(hBmp);

	HICON hIcon = CreateIconIndirect(&info);

	DrawIcon(hIcon);
	return;

	HWND hWndStatic = ::GetDlgItem(m_hWnd, m_DrawTargetID++);
	//SetStaticImageType(hWndStatic, SS_BITMAP);
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
	UINT	image_type = IMAGE_ICON;
	
	if (m_FilePath.IsEmpty())
	{
		m_MsgError.SetWindowText(TEXT("invalid path"));
		return;
	}

	LPCTSTR file_ext = PathFindExtension(m_FilePath);
	if(0 ==_tcsicmp(file_ext, TEXT(".exe")) || 0 == _tcsicmp(file_ext, TEXT(".dll")))
	{
		module = LoadLibrary(m_FilePath);
		if(!module)
		{
			m_MsgError.SetWindowText(TEXT("LoadLibrary fail"));
			return;
		}
	}
	else if(0 == _tcsicmp(file_ext, TEXT(".bmp")))
	{
		image_type = IMAGE_BITMAP;
	}

	DWORD fuLoad = 0;
	if(m_FuLoad_VGAColor) fuLoad |= LR_VGACOLOR;
	if(m_FuLoad_DefaultSize) fuLoad |= LR_DEFAULTSIZE;
	if(m_FuLoad_MONOCHROME) fuLoad |= LR_MONOCHROME;
	if(!module) fuLoad |= LR_LOADFROMFILE;

	HICON hIcon = (HICON)LoadImage(module, module ? MAKEINTRESOURCE(m_IconIdx) : (LPCTSTR)m_FilePath, image_type, m_IconSize, m_IconSize, fuLoad);

	if(hIcon)
	{
		if(IMAGE_ICON == image_type)
			DrawIcon(hIcon);
		else
			DrawIcon((HBITMAP)hIcon);
	}
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
		m_MsgError.SetWindowText(TEXT("can not get shell folder"));
		return;
	}

	ATL::CComPtr<IExtractImage> image_extracter;

	hr = shell_parent->GetUIObjectOf(NULL, 1, &pidl_child, IID_IExtractImage, NULL, (VOID**)&image_extracter);
	if(FAILED(hr))
	{
		m_MsgError.SetWindowText(TEXT("can not get IExtractImage object"));
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
	else
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

	HICON icon_large = NULL, icon_small = NULL;

	ret = (BOOL)SHGetFileInfo((LPCTSTR)pidl, NULL, &info, sizeof(info), SHGFI_ICON | SHGFI_LARGEICON | SHGFI_PIDL);

	if(info.hIcon)
	{
		icon_large = info.hIcon;
		DrawIcon(info.hIcon);
	}

	ret = (BOOL)SHGetFileInfo((LPCTSTR)pidl, NULL, &info, sizeof(info), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_PIDL);

	if(info.hIcon)
	{
		icon_small = info.hIcon;
		DrawIcon(info.hIcon);
	}

	SaveIcon3(TEXT("D:\\test1.ico"), &icon_small, 2);
}

void CIconSizeDlg::OnBnClickedShdefextract()
{
	UpdateData(TRUE);

	ResetDraw();

	if(!m_FilePath.IsEmpty())
	{
		HICON hIcon = NULL;
		HICON hIconSmall = NULL;
		HRESULT hr = SHDefExtractIcon(m_FilePath, m_IconIdx, 0 /*GIL_SIMULATEDOC*/, &hIcon, 0 /*&hIconSmall*/, m_IconSize);
		if(hIcon)
			DrawIcon(hIcon);
		else
			m_MsgError.SetWindowText(TEXT("SHDefExtractIcon fail"));

		if(hIconSmall)
			DrawIcon(hIconSmall);
	}
	else
	{
		m_MsgError.SetWindowText(TEXT("Please set file path"));
	}
}

void CIconSizeDlg::OnBnClickedGdipluscopy()
{
	UpdateData(TRUE);

	ResetDraw();

	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(m_FilePath);
	if(!bitmap)
	{
		printf("load image fail\n");
		return;
	}

	HICON hIcon = NULL;
	if(Gdiplus::Ok != bitmap->GetHICON(&hIcon))
	{
		printf("get icon fail\n");
		return;
	}

// 	HBITMAP hbmp=NULL;
// 	if(Gdiplus::Ok != bitmap->GetHBITMAP(Gdiplus::Color.Transparent, &hbmp))
// 	{
// 		printf("get bmp fail\n");
// 		return -1;
// 	}
// 
// 	hbmp = (HBITMAP)CopyImage(hbmp, IMAGE_BITMAP, 128, 128, LR_COPYDELETEORG | LR_COPYRETURNORG);
// 	if(!hbmp)
// 	{
// 		printf("copy bmp fail\n");
// 		return -1;
// 	}

	hIcon = (HICON)CopyImage(hIcon, IMAGE_ICON, 512, 512, LR_COPYRETURNORG | LR_COPYDELETEORG);
	if(!hIcon)
	{
		printf("copy icon fail\n");
		return;
	}

	DrawIcon(hIcon);
}

void CIconSizeDlg::OnBnClickedImagefactory()
{
	UpdateData(TRUE);

	ResetDraw();

	if(0 == m_IconSize) m_IconSize = 32;

	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(m_FilePath);
	if(!pidl)
	{
		return;
	}

	ATL::CComPtr<IShellItem> shell_item;
	HRESULT hr = SHCreateShellItem(NULL, NULL, pidl, &shell_item);

	ATL::CComPtr<IShellItemImageFactory> psiif;
	hr = shell_item->QueryInterface(IID_PPV_ARGS(&psiif));
	HBITMAP hThumbnail = 0;
	if (SUCCEEDED(hr))
	{
		const SIZE size = { m_IconSize, m_IconSize };
		hr = psiif->GetImage(size, SIIGBF_RESIZETOFIT, &hThumbnail);
		if(SUCCEEDED(hr))
		{
			DrawIcon(hThumbnail);
		}
	}
	return ;
}

GDIPlusInitialize::GDIPlusInitialize(void)
: gdiplusToken_(0)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);
}

GDIPlusInitialize::~GDIPlusInitialize(void)
{
	if(0 != gdiplusToken_)
		Gdiplus::GdiplusShutdown(gdiplusToken_);

}

