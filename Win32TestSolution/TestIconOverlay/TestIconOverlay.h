#ifndef TestIconOverlay_h__
#define TestIconOverlay_h__
#include <shobjidl.h>
#include <shlobj.h>

class CTestIconOverlay : public IShellExtInit, IShellIconOverlayIdentifier
{
public:
	CTestIconOverlay(void);

	STDMETHODIMP         QueryInterface(REFIID, LPVOID FAR *);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	virtual STDMETHODIMP Initialize( __in_opt PCIDLIST_ABSOLUTE pidlFolder, __in_opt IDataObject *pdtobj, __in_opt HKEY hkeyProgID );

	virtual STDMETHODIMP GetOverlayInfo(LPWSTR pwszIconFile, int cchMax, int *pIndex, DWORD *pdwFlags);
	virtual STDMETHODIMP GetPriority(int *pPriority);
	virtual STDMETHODIMP IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib);

protected:
	ULONG                m_cRef;
};



#endif // TestIconOverlay_h__

