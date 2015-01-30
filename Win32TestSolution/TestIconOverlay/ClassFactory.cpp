#include "stdafx.h"
#include "ClassFactory.h"
#include "TestIconOverlay.h"

extern long g_cDllRef;

CClassFactory::CClassFactory()
{
    m_cRef = 1;
    InterlockedIncrement(&g_cDllRef);
}

CClassFactory::~CClassFactory()
{
    InterlockedDecrement(&g_cDllRef);
}

//
// IUnknown
//
STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid) || IsEqualIID(IID_IClassFactory, riid))
    {
        *ppv = static_cast<IUnknown *>(this);
        AddRef();
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = NULL;
    }

    return hr;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}

//
// IClassFactory
//
STDMETHODIMP CClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
	//OutputDebugStringA("CClassFactory::CreateInstance\n");

    HRESULT hr = CLASS_E_NOAGGREGATION;

    if (!pUnkOuter)
    {
        hr = E_OUTOFMEMORY;

        CTestIconOverlay *pObj = new CTestIconOverlay();
        if (pObj)
        {
            hr = pObj->QueryInterface(riid, ppv);
            pObj->Release();
        }
    }

    return hr;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&g_cDllRef);
    }
    else
    {
        InterlockedDecrement(&g_cDllRef);
    }
    return S_OK;
}
