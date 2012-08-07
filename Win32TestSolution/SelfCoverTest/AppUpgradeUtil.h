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
	static void	ClearObsoleteFile(LPCTSTR log_file);
	static BOOL FileReplace(LPCTSTR target, LPCTSTR src, LPCTSTR log_file);
private:
	static BOOL ObsoleteFile(LPCTSTR target, LPCTSTR log_file);
	static INT	ClearObsoleteFile(LPCTSTR app_dir, BOOL clear_sub_dir);
	static BOOL WriteLog4ObsoleteFile(LPCTSTR target_file, LPCTSTR obsolete_file, LPCTSTR log_file);
};

#endif // _AppUpgradeUtil_h__
