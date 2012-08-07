/* ___________________________________________________________________________
@ 
@ file - e:\YunleDev\Library\cpl\AppUpgradeUtil.h
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-8-7
@ info -
@     
/* ___________________________________________________________________________*/
#ifndef _AppUpgradeUtil_h__
#define _AppUpgradeUtil_h__

#include "pugixml\pugixml.hpp"

class AppUpgradeUtil
{
public:
	AppUpgradeUtil();

	BOOL SetLogFileName(LPCTSTR file_name);
	LPCTSTR GetLogFileName() const { return update_log_name_; };
	BOOL SaveLogFile();

	INT	ClearObsoleteFile(LPCTSTR app_dir, BOOL clear_sub_dir);
	INT	ClearObsoleteFile();
	BOOL ObsoleteFile(LPCTSTR target);
	BOOL FileReplace(LPCTSTR target, LPCTSTR src);
	
protected:
	TCHAR update_log_name_[MAX_PATH + 1];
	pugi::xml_document update_log_xml_;
};

#endif // _AppUpgradeUtil_h__
