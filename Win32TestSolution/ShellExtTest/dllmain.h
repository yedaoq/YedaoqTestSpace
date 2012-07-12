// dllmain.h : 模块类的声明。

class CShellExtTestModule : public CAtlDllModuleT< CShellExtTestModule >
{
public :
	DECLARE_LIBID(LIBID_ShellExtTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SHELLEXTTEST, "{B4E0B4DB-7CA9-41E4-A5E9-F3B1DEBC37C2}")
};

extern class CShellExtTestModule _AtlModule;
