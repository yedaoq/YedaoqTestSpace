// ControlPanelItems.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <shobjidl.h>
#include <shlobj.h>


int _tmain(int argc, _TCHAR* argv[])
{
	ATL::CComPtr<IShellFolder> folder_desktop;
	HRESULT hr = SHGetDesktopFolder(&folder_desktop);

	PIDLIST_ABSOLUTE	pidl_cpl;
	hr = folder_desktop->ParseDisplayName(NULL, NULL, TEXT("::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\::{21EC2020-3AEA-1069-A2DD-08002B30309D}"), NULL, &pidl_cpl, NULL);

	ATL::CComPtr<IShellFolder> folder_cpl;
	hr = folder_desktop->BindToObject(pidl_cpl, NULL, IID_IShellFolder, (VOID**)&folder_cpl);


	return 0;
}

