#include <Windows.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	//setlocale(LC_ALL,"Chinese-simplified");
	APPBARDATA data = {0};
	data.cbSize = sizeof(data);
	data.hWnd = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	SHAppBarMessage(ABM_QUERYPOS, &data);
	return 0;
}

