// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <strsafe.h>
#include <commoncontrols.h>

HRESULT	DataObj_CopyHIDA(IDataObject *pdtobj, CIDA **ppida);

// declare a static CLIPFORMAT and pass that that by ref as the first param


// accessor helpers for the HIDA (aka CFSTR_SHELLIDLIST) clipboard format

// get the "folder" part of the IDList, this is the IDList that the children
// are rooted from
__inline PCUIDLIST_ABSOLUTE IDA_GetFolderIDList(CIDA const *pida)
{
	return (PCUIDLIST_ABSOLUTE)(((char *)pida) + pida->aoffset[0]);
}

// get the "child" item part of the IDList, these can be either child items or
// relative, hence PCUIDLIST_RELATIVE return type
__inline PCUIDLIST_RELATIVE IDA_GetItemIDList(CIDA const *pida, int i)
{
	return (PCUIDLIST_RELATIVE)(((char *)pida) + pida->aoffset[i + 1]);
}

// a CIDA structure represents a set of shell items, create the Nth item
// from that set in the form of an IShellItem
//
// this uses XP SP1 APIs so it works downlevel


__inline CLIPFORMAT GetClipboardFormat(CLIPFORMAT *pcf, PCWSTR pszForamt)
{
    if (*pcf == 0)
    {
        *pcf = (CLIPFORMAT)RegisterClipboardFormat(pszForamt);
    }
    return *pcf;
}

__inline HRESULT SetBlob(IDataObject *pdtobj, CLIPFORMAT cf, const void *pvBlob, UINT cbBlob)
{
    void *pv = GlobalAlloc(GPTR, cbBlob);
    HRESULT hr = pv ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr))
    {
        CopyMemory(pv, pvBlob, cbBlob);

        FORMATETC fmte = {cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

        // The STGMEDIUM structure is used to define how to handle a global memory transfer.
        // This structure includes a flag, tymed, which indicates the medium
        // to be used, and a union comprising pointers and a handle for getting whichever
        // medium is specified in tymed.
        STGMEDIUM medium = {};
        medium.tymed = TYMED_HGLOBAL;
        medium.hGlobal = pv;

        hr = pdtobj->SetData(&fmte, &medium, TRUE);
        if (FAILED(hr))
        {
            GlobalFree(pv);
        }
    }
    return hr;
}

__inline void SetDropTip(IDataObject *pdtobj, DROPIMAGETYPE type, PCWSTR pszMsg, PCWSTR pszInsert)
{
    DROPDESCRIPTION dd = { type };
    StringCchCopyW(dd.szMessage, ARRAYSIZE(dd.szMessage), pszMsg);
    StringCchCopyW(dd.szInsert, ARRAYSIZE(dd.szInsert), pszInsert ? pszInsert : L"");

    static CLIPFORMAT s_cfDropDescription = 0;
    SetBlob(pdtobj, GetClipboardFormat(&s_cfDropDescription, CFSTR_DROPDESCRIPTION), &dd, sizeof(dd));
}

__inline void ClearDropTip(IDataObject *pdtobj)
{
    SetDropTip(pdtobj, DROPIMAGE_INVALID, L"", NULL);
}

// helper to convert a data object with HIDA format or folder into a shell item
// note: if the data object contains more than one item this function will fail
// if you want to operate on the full selection use SHCreateShellItemArrayFromDataObject

HRESULT CreateItemFromObject(IUnknown *punk, REFIID riid, void **ppv)
{
    *ppv = NULL;

    PIDLIST_ABSOLUTE pidl = NULL;

#if CODE_WIN7
    HRESULT hr = SHGetIDListFromObject(punk, &pidl);
#else
	CIDA* pida = NULL;
	IDataObject* data_obj;
	HRESULT hr = punk->QueryInterface(IID_IDataObject, (VOID**)&data_obj);
	hr = DataObj_CopyHIDA(data_obj, &pida);

	if(SUCCEEDED(hr))
	{
		PIDLIST_ABSOLUTE pidlFolder; // needed for alignment
		hr = SHILCloneFull(IDA_GetFolderIDList(pida), &pidlFolder);

		if(SUCCEEDED(hr))
		{			
			hr = SHILCombine(pidlFolder, IDA_GetItemIDList(pida, 0), &pidl);
		}
	}
#endif

    if (SUCCEEDED(hr))
    {
        hr = SHCreateItemFromIDList(pidl, riid, ppv);
        ILFree(pidl);
    }
    else
    {
        // perhaps the input is from IE and if so we can construct an item from the URL
        IDataObject *pdo;
        hr = punk->QueryInterface(IID_PPV_ARGS(&pdo));
        if (SUCCEEDED(hr))
        {
            static CLIPFORMAT g_cfURL = 0;

            FORMATETC fmte = {GetClipboardFormat(&g_cfURL, CFSTR_SHELLURL), NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
            STGMEDIUM medium;
            hr = pdo->GetData(&fmte, &medium);
            if (SUCCEEDED(hr))
            {
                PCSTR pszURL = (PCSTR)GlobalLock(medium.hGlobal);
                if (pszURL)
                {
                    WCHAR szURL[2048];
                    SHAnsiToUnicode(pszURL, szURL, ARRAYSIZE(szURL));
                    hr = SHCreateItemFromParsingName(szURL, NULL, riid, ppv);
                    GlobalUnlock(medium.hGlobal);
                }
                ReleaseStgMedium(&medium);
            }
            pdo->Release();
        }
    }
    return hr;
}

// encapsulation of the shell drag drop presentation and Drop handling functionality
// this provides the following features 1) drag images, 2) drop tips,
// 3) ints OLE and registers drop target, 4) conversion of the data object item into shell items
//
// to use this
//      1) have the object that represents the main window of your app derive
//         from CDragDropHelper exposing it as public.
//         "class CAppClass : public CDragDropHelper"
//      2) add IDropTarget to the QueryInterface() implementation of your class
//         that is a COM object itself
//      3) in your WM_INITDIALOG handling call
//         InitializeDragDropHelper(_hdlg, DROPIMAGE_LINK, NULL) passing
//         the dialog window and drop tip template and type
//      4) in your WM_DESTROY handler add a call to TerminateDragDropHelper(). note not
//         doing this will lead to a leak of your object so this is important
//      5) add the delaration and implementation of OnDrop() to your class, this
//         gets called when the drop happens

class CDragDropHelper : public IDropTarget
{
public:
    CDragDropHelper() : _pdth(NULL), _pdtobj(NULL), _hrOleInit(OleInitialize(0)), _hwndRegistered(NULL), _dropImageType(DROPIMAGE_LABEL), _pszDropTipTemplate(NULL)
    {
        CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&_pdth));
    }

    ~CDragDropHelper()
    {
        SafeRelease(&_pdth);
        if (SUCCEEDED(_hrOleInit))
        {
            OleUninitialize();
        }
    }

    void SetDropTipTemplate(PCWSTR pszDropTipTemplate)
    {
        _pszDropTipTemplate = pszDropTipTemplate;
    }

    void InitializeDragDropHelper(HWND hwnd, DROPIMAGETYPE dropImageType, PCWSTR pszDropTipTemplate)
    {
        if (SUCCEEDED(_hrOleInit))
        {
            _dropImageType = dropImageType;
            _pszDropTipTemplate = pszDropTipTemplate;
            if (SUCCEEDED(RegisterDragDrop(hwnd, this)))
            {
                _hwndRegistered = hwnd;
            }
        }
    }

    void TerminateDragDropHelper()
    {
        if (_hwndRegistered)
        {
            RevokeDragDrop(_hwndRegistered);
            _hwndRegistered = NULL;
        }
    }

    HRESULT GetDragDropHelper(REFIID riid, void **ppv)
    {
        *ppv = NULL;
        return _pdth ? _pdth->QueryInterface(riid, ppv) : E_NOINTERFACE;
    }

    // direct access to the data object, if you don't want to use the shell item array
    IDataObject *GetDataObject()
    {
        return _pdtobj;
    }

    // IDropTarget
    IFACEMETHODIMP DragEnter(IDataObject *pdtobj, DWORD /* grfKeyState */, POINTL pt, DWORD *pdwEffect)
    {
        // leave *pdwEffect unchanged, we support all operations
        if (_pdth)
        {
            POINT ptT = { pt.x, pt.y };
            _pdth->DragEnter(_hwndRegistered, pdtobj, &ptT, *pdwEffect);
        }
        SetInterface(&_pdtobj, pdtobj);

        IShellItem *psi;
        HRESULT hr = CreateItemFromObject(pdtobj, IID_PPV_ARGS(&psi));
        if (SUCCEEDED(hr))
        {
            PWSTR pszName;
            hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
            if (SUCCEEDED(hr))
            {
                SetDropTip(pdtobj, _dropImageType, _pszDropTipTemplate ?  _pszDropTipTemplate : L"%1", pszName);
                CoTaskMemFree(pszName);
            }
            psi->Release();
        }
        return S_OK;
    }

    IFACEMETHODIMP DragOver(DWORD /* grfKeyState */, POINTL pt, DWORD *pdwEffect)
    {
        // leave *pdwEffect unchanged, we support all operations
        if (_pdth)
        {
            POINT ptT = { pt.x, pt.y };
            _pdth->DragOver(&ptT, *pdwEffect);
        }
        return S_OK;
    }

    IFACEMETHODIMP DragLeave()
    {
        if (_pdth)
        {
            _pdth->DragLeave();
        }
        ClearDropTip(_pdtobj);
        SafeRelease(&_pdtobj);
        return S_OK;
    }

    IFACEMETHODIMP Drop(IDataObject *pdtobj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
    {
        if (_pdth)
        {
            POINT ptT = { pt.x, pt.y };
            _pdth->Drop(pdtobj, &ptT, *pdwEffect);
        }

        IShellItemArray *psia;
        HRESULT hr = SHCreateShellItemArrayFromDataObject(_pdtobj, IID_PPV_ARGS(&psia));
        if (SUCCEEDED(hr))
        {
            OnDrop(psia, grfKeyState);
            psia->Release();
        }
        else
        {
            OnDropError(_pdtobj);
        }

        return S_OK;
    }

private:
    // client provides
    virtual HRESULT OnDrop(IShellItemArray *psia, DWORD grfKeyState) = 0;
    virtual HRESULT OnDropError(IDataObject * /* pdtobj */)
    {
        return S_OK;
    }

    IDropTargetHelper *_pdth;
    IDataObject *_pdtobj;
    DROPIMAGETYPE _dropImageType;
    PCWSTR _pszDropTipTemplate;
    HWND _hwndRegistered;
    HRESULT _hrOleInit;
};

HRESULT	DataObj_CopyHIDA(IDataObject *pdtobj, CIDA **ppida)
{
	*ppida = NULL;
	CLIPFORMAT format_idlist;

	FORMATETC fmte = {GetClipboardFormat(&format_idlist, CFSTR_SHELLIDLIST), NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM medium;
	HRESULT hr = pdtobj->GetData(&fmte, &medium);
	if (SUCCEEDED(hr))
	{
		void const *pSrc = GlobalLock(medium.hGlobal);
		size_t const cbSize = GlobalSize(medium.hGlobal);
		*ppida = (CIDA *)GlobalAlloc(GPTR, cbSize);
		hr = *ppida ? S_OK : E_OUTOFMEMORY;
		if (SUCCEEDED(hr))
		{
			memcpy(*ppida, pSrc, cbSize);
		}
		ReleaseStgMedium(&medium);
	}
	return hr;
}
