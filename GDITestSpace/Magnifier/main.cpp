#include <windows.h>
#include <wincodec.h>
#include <magnification.h>

#pragma comment(lib, "magnification.lib")

// 为简单起见，使用一个 固定的放大系数
#define MAGFACTOR  2.0f
#define RESTOREDWINDOWSTYLES WS_SIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_CAPTION | WS_MAXIMIZEBOX

//全局变量和字符串。
HINSTANCE           hInst;
const TCHAR         WindowClassName[]= TEXT("放大镜");
const TCHAR         WindowTitle[]= TEXT("屏幕放大镜例子");
const UINT          timerInterval = 16; //接近@ 60Hz的刷新率
HWND                hwndMag;
HWND                hwndHost;
RECT                magWindowRect;
RECT                hostWindowRect;

ATOM                RegisterHostWindowClass(HINSTANCE hInstance);
BOOL                SetupMagnifier(HINSTANCE hinst);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK       UpdateMagWindow(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void                GoFullScreen();
void                GoPartialScreen();
BOOL                isFullScreen = FALSE;

//初始化程序
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE /*hPrevInstance*/,
					 LPSTR     /*lpCmdLine*/,
					 int       nCmdShow)
{
	if (FALSE == MagInitialize())
	{
		return 0;
	}
	if (FALSE == SetupMagnifier(hInstance))
	{
		return 0;
	}

	ShowWindow(hwndHost, nCmdShow);
	UpdateWindow(hwndHost);

	// 创建时钟更新图像
	UINT_PTR timerId = SetTimer(hwndHost, 0, timerInterval, UpdateMagWindow);

	// 消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 关闭
	KillTimer(NULL, timerId);
	MagUninitialize();
	return (int) msg.wParam;
}

// 窗口程序的窗口承载放大镜

LRESULT CALLBACK HostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (isFullScreen)
			{
				GoPartialScreen();
			}
		}
		break;

	case WM_SYSCOMMAND:
		if (GET_SC_WPARAM(wParam) == SC_MAXIMIZE)
		{
			GoFullScreen();
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		if ( hwndMag != NULL )
		{
			GetClientRect(hWnd, &magWindowRect);
			// 重新设置窗口样式适应屏幕
			SetWindowPos(hwndMag, NULL,
				magWindowRect.left, magWindowRect.top, magWindowRect.right, magWindowRect.bottom, 0);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
//  注册放大镜窗口类
//
//  为窗口寄存器包含放大控制窗口类
//
ATOM RegisterHostWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = HostWndProc;
	wcex.hInstance      = hInstance;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(1 + COLOR_BTNFACE);
	wcex.lpszClassName  = WindowClassName;

	return RegisterClassEx(&wcex);
}

//
// 创建和初始化的窗口放大。
//
BOOL SetupMagnifier(HINSTANCE hinst)
{
	//设定的窗口界限根据屏幕大小的主窗口。
	hostWindowRect.top = 0;
	hostWindowRect.bottom = GetSystemMetrics(SM_CYSCREEN) / 4;  //屏幕顶部
	hostWindowRect.left = 0;
	hostWindowRect.right = GetSystemMetrics(SM_CXSCREEN);

	// 创建主窗口
	RegisterHostWindowClass(hinst);
	hwndHost = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,	// 必须是layered窗口
		WindowClassName, WindowTitle,
		RESTOREDWINDOWSTYLES,
		0, 0, hostWindowRect.right, hostWindowRect.bottom, NULL, NULL, hInst, NULL);
	if (!hwndHost)
	{
		return FALSE;
	}

	// 使窗口不透明。
	SetLayeredWindowAttributes(hwndHost, 0, 255, LWA_ALPHA);

	//创建一个放大镜控制填充图形区域
	GetClientRect(hwndHost, &magWindowRect);
	hwndMag = CreateWindow(WC_MAGNIFIER, TEXT("放大镜"),
		WS_CHILD | MS_SHOWMAGNIFIEDCURSOR | WS_VISIBLE,
		magWindowRect.left, magWindowRect.top, magWindowRect.right, magWindowRect.bottom, hwndHost, NULL, hInst, NULL );
	if (!hwndMag)
	{
		return FALSE;
	}

	// 设置放大系数。
	MAGTRANSFORM matrix;
	memset(&matrix, 0, sizeof(matrix));
	matrix.v[0][0] = MAGFACTOR;
	matrix.v[1][1] = MAGFACTOR;
	matrix.v[2][2] = 1.0f;

	BOOL ret = MagSetWindowTransform(hwndMag, &matrix);

	if (ret)
	{
		MAGCOLOREFFECT magEffectInvert =
		{{
			{ -1.0f,  0.0f,  0.0f,  0.0f,  0.0f },
			{  0.0f, -1.0f,  0.0f,  0.0f,  0.0f },
			{  0.0f,  0.0f, -1.0f,  0.0f,  0.0f },
			{  0.0f,  0.0f,  0.0f,  1.0f,  0.0f },
			{  1.0f,  1.0f,  1.0f,  0.0f,  1.0f }
		}};

		ret = MagSetColorEffect(hwndMag,&magEffectInvert);
	}

	return ret;
}

//
//随着时钟频率更新窗口
//
void CALLBACK UpdateMagWindow(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR /*idEvent*/, DWORD /*dwTime*/)
{
	POINT mousePoint;
	GetCursorPos(&mousePoint);

	int width = (int)((magWindowRect.right - magWindowRect.left) / MAGFACTOR);
	int height = (int)((magWindowRect.bottom - magWindowRect.top) / MAGFACTOR);
	RECT sourceRect;
	sourceRect.left = mousePoint.x - width / 2;
	sourceRect.top = mousePoint.y -  height / 2;

	//不突破屏幕区域
	if (sourceRect.left < 0)
	{
		sourceRect.left = 0;
	}
	if (sourceRect.left > GetSystemMetrics(SM_CXSCREEN) - width)
	{
		sourceRect.left = GetSystemMetrics(SM_CXSCREEN) - width;
	}
	sourceRect.right = sourceRect.left + width;

	if (sourceRect.top < 0)
	{
		sourceRect.top = 0;
	}
	if (sourceRect.top > GetSystemMetrics(SM_CYSCREEN) - height)
	{
		sourceRect.top = GetSystemMetrics(SM_CYSCREEN) - height;
	}
	sourceRect.bottom = sourceRect.top + height;

	// 设置为放大镜控件源矩形
	MagSetWindowSource(hwndMag, sourceRect);

	SetWindowPos(hwndHost, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );

	InvalidateRect(hwndMag, NULL, TRUE);
}

//全屏
void GoFullScreen()
{
	isFullScreen = TRUE;
	//风格的必须为适当的渲染分层窗口。
	//它是自称为透明，以便它不捕获鼠标点击
	SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	// 给窗口的系统菜单，以便它可以在任务栏关闭
	SetWindowLong(hwndHost, GWL_STYLE,  WS_CAPTION | WS_SYSMENU);

	// 计算显示区域跨度。
	HDC hDC = GetDC(NULL);
	int xSpan = GetSystemMetrics(SM_CXSCREEN);
	int ySpan = GetSystemMetrics(SM_CYSCREEN);
	ReleaseDC(NULL, hDC);

	// 计算系统元素的大小。
	int xBorder = GetSystemMetrics(SM_CXFRAME);
	int yCaption = GetSystemMetrics(SM_CYCAPTION);
	int yBorder = GetSystemMetrics(SM_CYFRAME);

	// 计算窗口起源和跨度全屏幕模式
	int xOrigin = -xBorder;
	int yOrigin = -yBorder - yCaption;
	xSpan += 2 * xBorder;
	ySpan += 2 * yBorder + yCaption;

	SetWindowPos(hwndHost, HWND_TOPMOST, xOrigin, yOrigin, xSpan, ySpan,
		SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}

//停靠模式

void GoPartialScreen()
{
	isFullScreen = FALSE;

	SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
	SetWindowLong(hwndHost, GWL_STYLE, RESTOREDWINDOWSTYLES);
	SetWindowPos(hwndHost, HWND_TOPMOST,
		hostWindowRect.left, hostWindowRect.top, hostWindowRect.right, hostWindowRect.bottom,
		SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}