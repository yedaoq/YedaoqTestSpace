// ControlPanelItems.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <shobjidl.h>
#include <shlobj.h>
#include <locale.h>

ATL::CComPtr<IShellFolder> folder_desktop;
ATL::CComPtr<IShellFolder> folder_cpl;

void PrintCplItemInfo(IShellItem* shell_item);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Chinese-simplified");
	CoInitialize(NULL);

	HRESULT hr = SHGetDesktopFolder(&folder_desktop);

	//LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}|::{BB64F8A7-BEE7-4E1A-AB8D-7D8273F7FDB6}");
	//LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}\5\::{BB64F8A7-BEE7-4E1A-AB8D-7D8273F7FDB6}");
	//LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}\0\::{BB64F8A7-BEE7-4E1A-AB8D-7D8273F7FDB6}");
	//LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}");
	//LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}\\5");
	LPTSTR path = TEXT("::{26EE0668-A00A-44D7-9371-BEB064C98683}\\0");

	PIDLIST_ABSOLUTE	pidl_cpl;
	hr = folder_desktop->ParseDisplayName(NULL, NULL, path, NULL, &pidl_cpl, NULL);	
	
	// 测试是否真的解析到了控制面板：创建ShellItem并检查它的名称
	ATL::CComPtr<IShellItem> shell_item_cpl;
	hr = SHCreateShellItem(NULL, NULL, pidl_cpl, &shell_item_cpl);
	LPWSTR display_name;
	shell_item_cpl->GetDisplayName(SIGDN_NORMALDISPLAY, &display_name);	

	hr = folder_desktop->BindToObject(pidl_cpl, NULL, IID_IShellFolder, (VOID**)&folder_cpl);

	ATL::CComPtr<IEnumIDList>  enum_cpl;
	hr = folder_cpl->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN | SHCONTF_NETPRINTERSRCH | SHCONTF_SHAREABLE | SHCONTF_STORAGE| SHCONTF_FASTITEMS | SHCONTF_FLATLIST, &enum_cpl);

	PITEMID_CHILD	item_pidl;
	while(SUCCEEDED(hr = enum_cpl->Next(1, &item_pidl, NULL)) && item_pidl)
	{
		if(item_pidl)
		{
			ATL::CComPtr<IShellItem> shell_item;
			if(SUCCEEDED(SHCreateShellItem(NULL, folder_cpl, item_pidl, &shell_item)))
			{
				PrintCplItemInfo(shell_item);
			}
		}
		else
		{
			printf("\n\nUnknow Item !\n\n");
		}
	}

	CoUninitialize();
	getchar();
	return 0;
}

void PrintDisplayName(IShellFolder* folder_parse, IShellItem* shell_item, int sigdn, LPCTSTR sigdn_name);
bool ParseDisplayName(IShellFolder* folder_parse, LPWSTR name);

void PrintCplItemInfo(IShellItem* shell_item)
{
	PrintDisplayName(NULL, shell_item, SIGDN_NORMALDISPLAY, TEXT("Display Name:\t\t"));
	PrintDisplayName(folder_desktop, shell_item, SIGDN_DESKTOPABSOLUTEPARSING, TEXT("DesktopParse:\t\t"));
	PrintDisplayName(folder_cpl, shell_item, SIGDN_PARENTRELATIVEPARSING, TEXT("ParentParse:\t\t"));
	PrintDisplayName(folder_desktop, shell_item, SIGDN_DESKTOPABSOLUTEEDITING, TEXT("DesktopEdit:\t\t"));
	PrintDisplayName(folder_cpl, shell_item, SIGDN_PARENTRELATIVEFORADDRESSBAR, TEXT("ParentAddress:\t\t"));
	printf("\n\n");
}

void PrintDisplayName( IShellFolder* folder_parse, IShellItem* shell_item, int sigdn, LPCTSTR sigdn_name )
{
	LPWSTR display_name;
	if (SUCCEEDED(shell_item->GetDisplayName((SIGDN)sigdn, &display_name)))
	{
		_tprintf(TEXT("%s%s\t\t"), sigdn_name, display_name);
		if (folder_parse)
		{
			_tprintf(TEXT("Parse:%s"), ParseDisplayName(folder_parse, display_name)? TEXT("able") : TEXT("unable"));
		}
		printf("\n");
		CoTaskMemFree(display_name);
	}
}

bool ParseDisplayName( IShellFolder* folder_parse, LPWSTR name )
{
	PIDLIST_RELATIVE pidl;
	HRESULT hr = folder_parse->ParseDisplayName(NULL, NULL, name, NULL, &pidl, NULL);
	ATL::CComPtr<IShellItem> item;
	if(SUCCEEDED(hr))
	{
		hr = SHCreateShellItem(NULL, folder_parse, pidl, &item);
		CoTaskMemFree(pidl);
	}

	return SUCCEEDED(hr);
}

