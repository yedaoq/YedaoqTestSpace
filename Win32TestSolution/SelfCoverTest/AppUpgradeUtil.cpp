#include "StdAfx.h"
#include "AppUpgradeUtil.h"
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#define OBSOLETEFILESUFFIX TEXT(".obsolete")

INT AppUpgradeUtil::ClearObsoleteFile(LPCTSTR app_dir, BOOL clear_sub_dir)
{
	if(!(app_dir))
		return -1;

	TCHAR file_match[MAX_PATH + 1];
	StrCpy(file_match, app_dir);
	::PathAppend(file_match, OBSOLETEFILESUFFIX);

	TCHAR file_name[MAX_PATH + 1];
	StrCpy(file_name, app_dir);

// 	SHFILEOPSTRUCT fo = {0};
// 	fo.wFunc = FO_DELETE;
// 	fo.pFrom = file_match;
// 	fo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
// 
// 	SHFileOperation(&fo);

	WIN32_FIND_DATA data={0};

	HANDLE file_find = FindFirstFile(file_match, &data);
	while(INVALID_HANDLE_VALUE != file_find)
	{
		::PathAppend(file_name, data.cFileName);
		::DeleteFile(file_name);
		::PathRemoveFileSpec(file_name);

		if(!FindNextFile(file_find, &data))
		{
			::FindClose(file_find);
			file_find = INVALID_HANDLE_VALUE;
			break;
		}
	}

	if(!clear_sub_dir) 
		return 1;

	::PathRemoveExtension(file_match);

	file_find = FindFirstFile(file_match, &data);
	while(INVALID_HANDLE_VALUE != file_find)
	{
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{			
			if(!(data.cFileName[0] == _T('.') && (data.cFileName[1] == _T('\0') || (data.cFileName[1] == _T('.') && data.cFileName[2] == _T('\0')))))
			{
				::PathAppend(file_name, data.cFileName);
				ClearObsoleteFile(file_name, clear_sub_dir);
				::PathRemoveFileSpec(file_name);
			}			
		}

		if(!FindNextFile(file_find, &data))
		{
			::FindClose(file_find);
			file_find = INVALID_HANDLE_VALUE;
			break;
		}
	}

	return 1;
}

void AppUpgradeUtil::ClearObsoleteFile(LPCTSTR log_file)
{
	if(!log_file || !log_file[0])
		return;

	pugi::xml_document xdoc;
	if(!xdoc.load_file(log_file))
		return;

	pugi::xml_node xnode = xdoc.child(TEXT("UpdateLog")).child(TEXT("ObsoleteFiles")).child(TEXT("File"));
	while(xnode)
	{
		pugi::xml_node xtemp = xnode;
		xnode = xnode.next_sibling(TEXT("File"));

		LPCTSTR file_name = xtemp.attribute(TEXT("Path")).value();
		if (!file_name || !file_name[0] || ::DeleteFile(xtemp.attribute(TEXT("Path")).value()) || 0 != _taccess(file_name, 0))
		{
			xtemp.parent().remove_child(xtemp);
		}
	}
	xdoc.save_file(log_file);
}

BOOL AppUpgradeUtil::FileReplace( LPCTSTR target, LPCTSTR src, LPCTSTR log_file )
{
	if(target)
	{
		if(0 == _taccess(target, 0))	// if target exist, try to remove it
		{
			if(!ObsoleteFile(target, log_file))
				return FALSE;
		}
	}

	if(src)
	{
		return ::MoveFile(src, target);
	}

	return TRUE;
}

BOOL AppUpgradeUtil::ObsoleteFile( LPCTSTR target, LPCTSTR log_file )
{
	TCHAR file_name[MAX_PATH + 1];
	StrCpy(file_name, target);
	::PathRemoveFileSpec(file_name);
	GetTempFileName(file_name, TEXT("~tmp"), 0, file_name);
	::DeleteFile(file_name);
	BOOL bRet = ::MoveFile(target, file_name);
	if(bRet)
	{
		WriteLog4ObsoleteFile(target, file_name, log_file);
	}
	return bRet;
}

BOOL AppUpgradeUtil::WriteLog4ObsoleteFile(LPCTSTR target_file, LPCTSTR obsolete_file, LPCTSTR log_file )
{
	if(!obsolete_file || !log_file || !log_file[0])
		return FALSE;

	pugi::xml_document xdoc;
	xdoc.load_file(log_file);
	pugi::xml_node xnode = xdoc.child(TEXT("UpdateLog"));

	if(!xnode)
	{
		xnode = xdoc.append_child(TEXT("UpdateLog"));
		xnode = xnode.append_child(TEXT("ObsoleteFiles"));
	}
	else
	{
		if(!xnode.child(TEXT("ObsoleteFiles")))
			xnode = xnode.append_child(TEXT("ObsoleteFiles"));
		else
			xnode = xnode.child(TEXT("ObsoleteFiles"));
	}

	xnode = xnode.append_child(TEXT("File"));
	xnode.append_attribute(TEXT("Path")).set_value(obsolete_file);
	xnode.append_attribute(TEXT("target")).set_value(target_file);

	xdoc.save_file(log_file);

	return TRUE;
}
