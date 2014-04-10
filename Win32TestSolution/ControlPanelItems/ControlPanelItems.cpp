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

	HRESULT hr = SHGetDesktopFolder(&folder_desktop);

	PIDLIST_ABSOLUTE	pidl_cpl;
	hr = folder_desktop->ParseDisplayName(NULL, NULL, TEXT("::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\::{21EC2020-3AEA-1069-A2DD-08002B30309D}"), NULL, &pidl_cpl, NULL);
	
	// 测试是否真的解析到了控制面板：创建ShellItem并检查它的名称
	ATL::CComPtr<IShellItem> shell_item_cpl;
	SHCreateShellItem(NULL, folder_desktop, pidl_cpl, &shell_item_cpl);
	LPWSTR display_name;
	shell_item_cpl->GetDisplayName(SIGDN_NORMALDISPLAY, &display_name);	// display_name : 我的电脑
	// 结果：folder_desktop->ParseDisplayName只解析了一层路径，没有往下解析或解析失败了

	ATL::CComPtr<IShellFolder>	folder_computer;
	hr = folder_desktop->BindToObject(pidl_cpl, NULL, IID_IShellFolder, (VOID**)&folder_computer);
	hr = folder_computer->ParseDisplayName(NULL, NULL, TEXT("::{21EC2020-3AEA-1069-A2DD-08002B30309D}"), NULL, &pidl_cpl, NULL);
	hr = folder_computer->BindToObject(pidl_cpl, NULL, IID_IShellFolder, (VOID**)&folder_cpl);

	ATL::CComPtr<IEnumIDList>  enum_cpl;
	hr = folder_cpl->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &enum_cpl);

	PITEMID_CHILD	item_pidl;
	while(SUCCEEDED(enum_cpl->Next(1, &item_pidl, NULL)) && item_pidl)
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

