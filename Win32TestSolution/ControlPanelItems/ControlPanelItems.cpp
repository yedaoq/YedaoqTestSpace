// ControlPanelItems.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <shobjidl.h>
#include <shlobj.h>

ATL::CComPtr<IShellFolder> folder_desktop;
ATL::CComPtr<IShellFolder> folder_cpl;

void PrintCplItemInfo(IShellItem* shell_item);

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = SHGetDesktopFolder(&folder_desktop);

	PIDLIST_ABSOLUTE	pidl_cpl;
	hr = folder_desktop->ParseDisplayName(NULL, NULL, TEXT("::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\::{21EC2020-3AEA-1069-A2DD-08002B30309D}"), NULL, &pidl_cpl, NULL);
	
	hr = folder_desktop->BindToObject(pidl_cpl, NULL, IID_IShellFolder, (VOID**)&folder_cpl);

	ATL::CComPtr<IEnumIDList>  enum_cpl;
	hr = folder_cpl->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &enum_cpl);

	PITEMID_CHILD	item_pidl;
	while(SUCCEEDED(enum_cpl->Next(1, &item_pidl, NULL)))
	{
		ATL::CComPtr<IShellItem> shell_item;
		if(SUCCEEDED(SHCreateShellItem(NULL, folder_cpl, item_pidl, &shell_item)))
			PrintCplItemInfo(shell_item);
	}

	return 0;
}

void PrintDisplayName(IShellFolder* folder_parse, IShellItem* shell_item, int sigdn, LPCTSTR sigdn_name);

void PrintCplItemInfo(IShellItem* shell_item)
{
	PrintDisplayName(NULL, shell_item, SIGDN_NORMALDISPLAY, TEXT("Display Name:\t\t"));
	PrintDisplayName(folder_desktop, shell_item, SIGDN_DESKTOPABSOLUTEPARSING, TEXT("DesktopParse:\t\t"));
	PrintDisplayName(folder_cpl, shell_item, SIGDN_PARENTRELATIVEPARSING, TEXT("ParentParse:\t\t"));
	PrintDisplayName(folder_desktop, shell_item, SIGDN_DESKTOPABSOLUTEEDITING, TEXT("DesktopEdit:\t\t"));
	PrintDisplayName(folder_cpl, shell_item, SIGDN_PARENTRELATIVEFORADDRESSBAR, TEXT("ParentAddress:\t\t"));
}

void PrintDisplayName( IShellFolder* folder_parse, IShellItem* shell_item, int sigdn, LPCTSTR sigdn_name )
{
	LPWSTR display_name;
	if (SUCCEEDED(shell_item->GetDisplayName(sigdn, &display_name)))
	{
		_tprintf(TEXT("%s"))
	}
}

