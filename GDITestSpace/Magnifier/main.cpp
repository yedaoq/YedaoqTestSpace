#include <windows.h>
#include <wincodec.h>
#include <magnification.h>

#pragma comment(lib, "magnification.lib")

// Ϊ�������ʹ��һ�� �̶��ķŴ�ϵ��
#define MAGFACTOR  2.0f
#define RESTOREDWINDOWSTYLES WS_SIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_CAPTION | WS_MAXIMIZEBOX

//ȫ�ֱ������ַ�����
HINSTANCE           hInst;
const TCHAR         WindowClassName[]= TEXT("�Ŵ�");
const TCHAR         WindowTitle[]= TEXT("��Ļ�Ŵ�����");
const UINT          timerInterval = 16; //�ӽ�@ 60Hz��ˢ����
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

//��ʼ������
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

	// ����ʱ�Ӹ���ͼ��
	UINT_PTR timerId = SetTimer(hwndHost, 0, timerInterval, UpdateMagWindow);

	// ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// �ر�
	KillTimer(NULL, timerId);
	MagUninitialize();
	return (int) msg.wParam;
}

// ���ڳ���Ĵ��ڳ��طŴ�

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
			// �������ô�����ʽ��Ӧ��Ļ
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
//  ע��Ŵ󾵴�����
//
//  Ϊ���ڼĴ��������Ŵ���ƴ�����
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
// �����ͳ�ʼ���Ĵ��ڷŴ�
//
BOOL SetupMagnifier(HINSTANCE hinst)
{
	//�趨�Ĵ��ڽ��޸�����Ļ��С�������ڡ�
	hostWindowRect.top = 0;
	hostWindowRect.bottom = GetSystemMetrics(SM_CYSCREEN) / 4;  //��Ļ����
	hostWindowRect.left = 0;
	hostWindowRect.right = GetSystemMetrics(SM_CXSCREEN);

	// ����������
	RegisterHostWindowClass(hinst);
	hwndHost = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,	// ������layered����
		WindowClassName, WindowTitle,
		RESTOREDWINDOWSTYLES,
		0, 0, hostWindowRect.right, hostWindowRect.bottom, NULL, NULL, hInst, NULL);
	if (!hwndHost)
	{
		return FALSE;
	}

	// ʹ���ڲ�͸����
	SetLayeredWindowAttributes(hwndHost, 0, 255, LWA_ALPHA);

	//����һ���Ŵ󾵿������ͼ������
	GetClientRect(hwndHost, &magWindowRect);
	hwndMag = CreateWindow(WC_MAGNIFIER, TEXT("�Ŵ�"),
		WS_CHILD | MS_SHOWMAGNIFIEDCURSOR | WS_VISIBLE,
		magWindowRect.left, magWindowRect.top, magWindowRect.right, magWindowRect.bottom, hwndHost, NULL, hInst, NULL );
	if (!hwndMag)
	{
		return FALSE;
	}

	// ���÷Ŵ�ϵ����
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
//����ʱ��Ƶ�ʸ��´���
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

	//��ͻ����Ļ����
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

	// ����Ϊ�Ŵ󾵿ؼ�Դ����
	MagSetWindowSource(hwndMag, sourceRect);

	SetWindowPos(hwndHost, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );

	InvalidateRect(hwndMag, NULL, TRUE);
}

//ȫ��
void GoFullScreen()
{
	isFullScreen = TRUE;
	//���ı���Ϊ�ʵ�����Ⱦ�ֲ㴰�ڡ�
	//�����Գ�Ϊ͸�����Ա��������������
	SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	// �����ڵ�ϵͳ�˵����Ա����������������ر�
	SetWindowLong(hwndHost, GWL_STYLE,  WS_CAPTION | WS_SYSMENU);

	// ������ʾ�����ȡ�
	HDC hDC = GetDC(NULL);
	int xSpan = GetSystemMetrics(SM_CXSCREEN);
	int ySpan = GetSystemMetrics(SM_CYSCREEN);
	ReleaseDC(NULL, hDC);

	// ����ϵͳԪ�صĴ�С��
	int xBorder = GetSystemMetrics(SM_CXFRAME);
	int yCaption = GetSystemMetrics(SM_CYCAPTION);
	int yBorder = GetSystemMetrics(SM_CYFRAME);

	// ���㴰����Դ�Ϳ��ȫ��Ļģʽ
	int xOrigin = -xBorder;
	int yOrigin = -yBorder - yCaption;
	xSpan += 2 * xBorder;
	ySpan += 2 * yBorder + yCaption;

	SetWindowPos(hwndHost, HWND_TOPMOST, xOrigin, yOrigin, xSpan, ySpan,
		SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}

//ͣ��ģʽ

void GoPartialScreen()
{
	isFullScreen = FALSE;

	SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
	SetWindowLong(hwndHost, GWL_STYLE, RESTOREDWINDOWSTYLES);
	SetWindowPos(hwndHost, HWND_TOPMOST,
		hostWindowRect.left, hostWindowRect.top, hostWindowRect.right, hostWindowRect.bottom,
		SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}