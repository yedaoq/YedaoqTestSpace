#include "stdafx.h"
#include <strsafe.h> // for StringCchXXX functions
#include <olectl.h> // for SELFREG_E_CLASS
#include <shlobj.h> // for ICatRegister
#include "ClassFactory.h" // for the class factory
#include <shlwapi.h>

// {6B348EA0-8482-4a33-A441-75D3C7023065}
CLSID CLSID_IconOverlay = 
{ 0x6b348ea0, 0x8482, 0x4a33, { 0xa4, 0x41, 0x75, 0xd3, 0xc7, 0x2, 0x30, 0x65 } };

HINSTANCE   g_hInst     = NULL;
long        g_cDllRef   = 0;

STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void *)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		CoInitialize(NULL);
        g_hInst = hInstance;
        DisableThreadLibraryCalls(hInstance);
		//OutputDebugStringA("CTestIconOverlay Dll Load\n");
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	//OutputDebugStringA("DllGetClassObject\n");

    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(CLSID_IconOverlay, rclsid))
    {
        hr = E_OUTOFMEMORY;

        CClassFactory *pClassFactory = new CClassFactory();
        if (pClassFactory)
        {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }

    return hr;
}

STDAPI DllCanUnloadNow()
{
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}

HRESULT RegisterServer()
{
    WCHAR szCLSID[MAX_PATH];
    StringFromGUID2(CLSID_IconOverlay, szCLSID, ARRAYSIZE(szCLSID));

    WCHAR szSubkey[MAX_PATH];
    HKEY hKey;

    HRESULT hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        hr = E_FAIL;
        if (ERROR_SUCCESS == RegCreateKeyExW(HKEY_CLASSES_ROOT, szSubkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))
        {
            WCHAR const szName[] = L"IconOverlay Test";
            if (ERROR_SUCCESS == RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE) szName, sizeof(szName)))
            {
                hr = S_OK;
            }

            RegCloseKey(hKey);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s\\InprocServer32", szCLSID);
        if (SUCCEEDED(hr))
        {
            hr = HRESULT_FROM_WIN32(RegCreateKeyExW(HKEY_CLASSES_ROOT, szSubkey,
                 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL));
            if (SUCCEEDED(hr))
            {
                WCHAR szModule[MAX_PATH];
                if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)))
                {
                    DWORD cch = lstrlen(szModule);
                    hr = HRESULT_FROM_WIN32(RegSetValueExW(hKey, NULL, 0, REG_SZ, (LPBYTE) szModule, cch * sizeof(szModule[0])));
                }

                if (SUCCEEDED(hr))
                {
                    WCHAR const szModel[] = L"Apartment";
                    hr = HRESULT_FROM_WIN32(RegSetValueExW(hKey, L"ThreadingModel", 0,  REG_SZ, (LPBYTE) szModel, sizeof(szModel)));
                }

                RegCloseKey(hKey);
            }
        }
    }

	SHSetValue(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers\\IconOverlayTest"), NULL, REG_SZ, szCLSID, _tcslen(szCLSID) * sizeof(TCHAR));

    return hr;
}

HRESULT RegisterComCat()
{
    ICatRegister *pcr;
    HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pcr));
    if (SUCCEEDED(hr))
    {
        CATID catid = CATID_DeskBand;
        hr = pcr->RegisterClassImplCategories(CLSID_IconOverlay, 1, &catid);
        pcr->Release();
    }
    return hr;
}

STDAPI DllRegisterServer()
{
    // Register the deskband object.
    HRESULT hr = RegisterServer();
    if (SUCCEEDED(hr))
    {
        // Register the component category for the deskband object.
        hr = RegisterComCat();
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}

STDAPI DllUnregisterServer()
{
    WCHAR szCLSID[MAX_PATH];
    StringFromGUID2(CLSID_IconOverlay, szCLSID, ARRAYSIZE(szCLSID));

    WCHAR szSubkey[MAX_PATH];
    HRESULT hr = StringCchPrintfW(szSubkey, ARRAYSIZE(szSubkey), L"CLSID\\%s", szCLSID);
    if (SUCCEEDED(hr))
    {
        if (ERROR_SUCCESS != SHDeleteKey(HKEY_CLASSES_ROOT, szSubkey))
        {
            hr = E_FAIL;
        }
    }

    return SUCCEEDED(hr) ? S_OK : SELFREG_E_CLASS;
}
