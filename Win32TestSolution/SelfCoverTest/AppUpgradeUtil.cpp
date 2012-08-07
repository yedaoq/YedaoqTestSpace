#include "StdAfx.h"
#include "AppUpgradeUtil.h"
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#define OBSOLETEFILESUFFIX TEXT("*.obsolete")

struct scoped_current_dir
{
	scoped_current_dir(LPCTSTR tmp_cur_dir)
	{
		GetCurrentDirectory(MAX_PATH, origin_cur_dir);
		origin_cur_dir[MAX_PATH] = '\0';
		SetCurrentDirectory(tmp_cur_dir);
	}

	~scoped_current_dir()
	{
		SetCurrentDirectory(origin_cur_dir);
	}

protected:
	TCHAR origin_cur_dir[MAX_PATH + 1];
};

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

INT AppUpgradeUtil::ClearObsoleteFile()
{	
	pugi::xml_node xnode = update_log_xml_.root().child(TEXT("UpdateLog")).child(TEXT("ObsoleteFiles"));
	xnode = xnode.child(TEXT("File"));
	while(xnode)
	{
		pugi::xml_node xtemp = xnode;
		xnode = xnode.next_sibling(TEXT("File"));
		::DeleteFile(xtemp.attribute(TEXT("Path")).value());
		xtemp.parent().remove_child(xtemp);
	}
	return 0;
}

BOOL AppUpgradeUtil::FileReplace( LPCTSTR target, LPCTSTR src )
{
	if(target)
	{
		if(0 == _taccess(target, 0))	// if target exist, try to remove it
		{
			if(NULL != GetModuleHandle(target))
			{
				ObsoleteFile(target);
			}
			else if(!::DeleteFile(target))
			{
				ObsoleteFile(target);
			}
		}
		
		if(0 == _taccess(target, 0))	// if remove fault, then fail
			return FALSE;
	}

	if(src)
	{
		return ::MoveFile(src, target);
	}

	return FALSE;
}

BOOL AppUpgradeUtil::ObsoleteFile( LPCTSTR target )
{
	TCHAR file_name[MAX_PATH + 1];
	StrCpy(file_name, target);
	::PathRemoveFileSpec(file_name);
	GetTempFileName(file_name, TEXT("~tmp"), 0, file_name);
	::DeleteFile(file_name);
	BOOL bRet = ::MoveFile(target, file_name);
	if(bRet)
	{
		pugi::xml_node xnode = update_log_xml_.root().child(TEXT("UpdateLog")).child(TEXT("ObsoleteFiles"));
		xnode = xnode.append_child(TEXT("File"));
		pugi::xml_attribute xattr = xnode.append_attribute(TEXT("Path"));
		xattr.set_value(file_name);
	}
	return bRet;
}

AppUpgradeUtil::AppUpgradeUtil()
{
	update_log_name_[0] = 0;
}

BOOL AppUpgradeUtil::SetLogFileName( LPCTSTR file_name )
{
	if(!file_name || !file_name[0])
		return FALSE;
	
	StrCpy(update_log_name_, file_name);

	if(!update_log_xml_.load_file(file_name))
	{
		update_log_xml_.reset();
	}
	
	pugi::xml_node xnode = update_log_xml_.root();
	if(!update_log_xml_.root().child(TEXT("UpdateLog")))
	{
		xnode = update_log_xml_.root().append_child(TEXT("UpdateLog"));
	}
	else
	{
		xnode = update_log_xml_.root().child(TEXT("UpdateLog"));
	}

	if(!xnode.child(TEXT("ObsoleteFiles")))
	{
		xnode = xnode.append_child(TEXT("ObsoleteFiles"));
	}
	else
	{
		xnode = xnode.child(TEXT("ObsoleteFiles"));
	}

	return TRUE;
}

BOOL AppUpgradeUtil::SaveLogFile()
{
	return update_log_xml_.save_file(update_log_name_);
}