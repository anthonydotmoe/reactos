#include "precomp.h"

// Helper functions to translate a guid to a readable name
bool GetInterfaceName(const WCHAR* InterfaceString, WCHAR* buf, size_t size)
{
    WCHAR LocalBuf[100];
    DWORD dwType = 0, dwDataSize = size * sizeof(WCHAR);

    if (!SUCCEEDED(StringCchPrintfW(LocalBuf, _countof(LocalBuf), L"Interface\\%s", InterfaceString)))
        return false;

    return RegGetValueW(HKEY_CLASSES_ROOT, LocalBuf, NULL, RRF_RT_REG_SZ, &dwType, buf, &dwDataSize) == ERROR_SUCCESS;
}
std::wstring g2s(REFCLSID iid)
{
    static WCHAR buf[2][300];
    static int idx = 0;

    idx ^= 1;

    LPOLESTR tmp;
    HRESULT hr = ProgIDFromCLSID(iid, &tmp);
    if (SUCCEEDED(hr))
    {
        wcscpy(buf[idx], tmp);
        CoTaskMemFree(tmp);
        return buf[idx];
    }
    StringFromGUID2(iid, buf[idx], _countof(buf[idx]));
    if (GetInterfaceName(buf[idx], buf[idx], _countof(buf[idx])))
    {
        return buf[idx];
    }
    StringFromGUID2(iid, buf[idx], _countof(buf[idx]));

    return std::wstring(buf[idx]);
}


// ISnapinAbout

HRESULT AllocateString(LPCOLESTR str, LPOLESTR *pStrOut)
{
    if (pStrOut == nullptr)
    {
        return E_POINTER;
    }

    if (str == nullptr)
    {
        return E_INVALIDARG;
    }

    size_t len = wcslen(str) + 1; // Length of string plus null

    // Allocate memory for the string
    *pStrOut = static_cast<LPOLESTR>(CoTaskMemAlloc(len * sizeof(wchar_t)));
    if (*pStrOut == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    // Copy the string into the allocated memory
    wcscpy(*pStrOut, str);

    return S_OK;
}

STDMETHODIMP CFolderSnapin::GetSnapinDescription(LPOLESTR *lpDescription)
{
    Logger::Instance() << "CFolderSnapin::ISnapinAbout::GetSnapinDescription" << Endl;
    const wchar_t *desc = L"Folder allows you to group snap-ins logically.";
    return AllocateString(desc, lpDescription);
}

STDMETHODIMP CFolderSnapin::GetProvider(LPOLESTR *lpName)
{
    Logger::Instance() << "CFolderSnapin::ISnapinAbout::GetProvider" << Endl;
    const wchar_t *name = L"ReactOS Team";
    return AllocateString(name, lpName);
}

STDMETHODIMP CFolderSnapin::GetSnapinVersion(LPOLESTR *lpVersion)
{
    Logger::Instance() << "CFolderSnapin::ISnapinAbout::GetSnapinVersion" << Endl;
    const wchar_t *version = L"1.0.0";
    return AllocateString(version, lpVersion);
}

STDMETHODIMP CFolderSnapin::GetSnapinImage(HICON *hAppIcon)
{
    Logger::Instance() << "CFolderSnapin::ISnapinAbout::GetSnapinImage" << Endl;
    return E_NOTIMPL;
}

STDMETHODIMP CFolderSnapin::GetStaticFolderImage(HBITMAP *hSmallImage, HBITMAP *hSmallImageOpen, HBITMAP *hLargeImage, COLORREF *cMask)
{
    Logger::Instance() << "CFolderSnapin::ISnapinAbout::GetStaticFolderImage" << Endl;
    return E_NOTIMPL;
}

// IComponentData

template<typename T>
void **ppvarg_helper(T** pp)
{
    (void)static_cast<IUnknown*>(*pp); // Derive from IUnknown
    return reinterpret_cast<void**>(pp);
}

STDMETHODIMP CFolderSnapin::Initialize(LPUNKNOWN lpUnknown)
{
    
/*
Snap-in entry point. The snap-in should query the console for its
IConsoleNamespace and IConsole interfaces. Also consider adding an image strip
using the IImageList::ImageListSetStrip method.
*/

    Logger::Instance() << "(0x" << std::hex << this << ")CFolderSnapin::Initialize called." << Endl;
    HRESULT hr;
    CComPtr<IUnknown> pMMCUnknown = CComPtr<IUnknown>(lpUnknown);
    hr = pMMCUnknown->QueryInterface(IID_IConsoleNameSpace, ppvarg_helper(&m_pConsoleNameSpace));
    Logger::Instance() << "CFolderSnapin::IComponentData::Initialize: lpUnknown->QI for IConsoleNamespace: " << hr << Endl;
    return S_OK;
}

STDMETHODIMP CFolderSnapin::CreateComponent(LPCOMPONENT *ppComponent)
{
    Logger::Instance() << "CFolderSnapin::IComponentData::CreateComponent called." << Endl;
    // Create a new instance of CFolderComponentImpl
    CComObject<CFolderComponentImpl> *pComponent;
    HRESULT hr = CComObject<CFolderComponentImpl>::CreateInstance(&pComponent);
    if (FAILED(hr))
        return hr;
    
    // Initialize the component with a pointer to the snap-in
    pComponent->SetSnapin(this);
    
    // Add a reference and return the component
    pComponent->AddRef();
    *ppComponent = pComponent;

    return S_OK;
}

STDMETHODIMP CFolderSnapin::Notify(LPDATAOBJECT, MMC_NOTIFY_TYPE type, LPARAM, LPARAM)
{
    Logger::Instance() << "CFolderSnapin::IComponentData::Notify " << type << Endl;
    return S_FALSE;
}

STDMETHODIMP CFolderSnapin::Destroy()
{
    // Release all references to the console
    Logger::Instance() << "CFolderSnapin::IComponentData::Destroy" << Endl;
    m_pConsoleNameSpace.Release();
    m_pConsole.Release();
    return S_OK;
}

STDMETHODIMP CFolderSnapin::QueryDataObject(MMC_COOKIE cookie, DATA_OBJECT_TYPES type, LPDATAOBJECT *ppDataObject)
{
    Logger::Instance() << "CFolderSnapin::IComponentData::QueryDataObject(cookie: " << cookie << ", type: 0x" << std::hex << type << ")" << Endl;
    if (type == DATA_OBJECT_TYPES::CCT_SNAPIN_MANAGER)
    {
        // Display the "Connect to server dialog" if needed.
        //MessageBox(NULL, TEXT("You're adding the Folder snap-in."), NULL, 0);
    }
    if (type == DATA_OBJECT_TYPES::CCT_SCOPE || type == DATA_OBJECT_TYPES::CCT_SNAPIN_MANAGER)
    {
        switch (cookie)
        {
        case 0:
            // This is the static node
            {
                *ppDataObject = static_cast<IDataObject*>(this);
                AddRef();
                return S_OK;
            }
            break;
        
        default:
            break;
        }
    }
    return E_NOTIMPL;
}

STDMETHODIMP CFolderSnapin::GetDisplayInfo(SCOPEDATAITEM *pScopeDataItem)
{
    Logger::Instance() << "CFolderSnapin::IComponentData::GetDisplayInfo" << Endl;
    return E_NOTIMPL;
}

STDMETHODIMP CFolderSnapin::CompareObjects(LPDATAOBJECT, LPDATAOBJECT)
{
    Logger::Instance() << "CFolderSnapin::IComponentData::CompareObjects" << Endl;
    return E_NOTIMPL;
}

/*
STDMETHODIMP CFolderSnapin::QueryDispatch(MMC_COOKIE, DATA_OBJECT_TYPES, LPDISPATCH*)
{
    Logger::Instance() << "CFolderSnapin::IComponentData2::QueryDispatch" << Endl;
    return E_NOTIMPL;
}
*/

// IComponent

STDMETHODIMP CFolderComponentImpl::Initialize(LPCONSOLE lpConsole)
{
/*
When the CreateComponent method of IComponentData is called, the snap-in can
create an instance of the IComponent interface. MMC calls CreateComponent each
time a new view (MDI child window) is created; each view is associated with a
different IComponent object.
*/
    // Use the provided IConsole interface to QI for IConsole2, IResultData, etc.
    Logger::Instance() << "\t(0x" << std::hex << this << ")CFolderComponentImpl::Initialize called." << Endl;
    return S_OK;
}

STDMETHODIMP CFolderComponentImpl::Notify(LPDATAOBJECT, MMC_NOTIFY_TYPE event, LPARAM, LPARAM)
{
    Logger::Instance() << "\tCFolderComponentImpl::Notify(event: 0x" << std::hex << event << ")" << Endl;
    return S_FALSE;
}

STDMETHODIMP CFolderComponentImpl::Destroy(MMC_COOKIE)
{
    Logger::Instance() << "\tCFolderComponentImpl::Destroy called." << Endl;
    return S_OK;
}

STDMETHODIMP CFolderComponentImpl::QueryDataObject(MMC_COOKIE cookie, DATA_OBJECT_TYPES type, LPDATAOBJECT *ppDataObject)
{
    Logger::Instance() << "\tCFolderComponentImpl::QueryDataObject(cookie: " << cookie << ", type: 0x" << std::hex << type << ")" << Endl;
    return E_NOTIMPL;
}

STDMETHODIMP CFolderComponentImpl::GetResultViewType(MMC_COOKIE cookie, LPOLESTR *ppViewType, long *pViewOptions)
{
    Logger::Instance() << "\tCFolderComponentImpl::GetResultViewType called. Returning \"standard list view\"" << Endl;
    if (ppViewType == nullptr || pViewOptions == nullptr)
    {
        Logger::Instance() << "\tinput pointer was null... return E_POINTER" << Endl;
        return E_POINTER;
    }
    *ppViewType = NULL;
    *pViewOptions = MMC_VIEW_OPTIONS_NONE;
    return S_OK;
}

STDMETHODIMP CFolderComponentImpl::GetDisplayInfo(RESULTDATAITEM*)
{
    Logger::Instance() << "\tCFolderComponentImpl::GetDisplayInfo called." << Endl;
    return E_NOTIMPL;
}

STDMETHODIMP CFolderComponentImpl::CompareObjects(LPDATAOBJECT, LPDATAOBJECT)
{
    Logger::Instance() << "\tCFolderComponentImpl::CompareObjects called." << Endl;
    return E_NOTIMPL;
}

/*
STDMETHODIMP CFolderComponentImpl::QueryDispatch(MMC_COOKIE, DATA_OBJECT_TYPES, LPDISPATCH*)
{
    return E_NOTIMPL;
}

STDMETHODIMP CFolderComponentImpl::GetResultViewType2(MMC_COOKIE, PRESULT_VIEW_TYPE_INFO)
{
    return S_FALSE;
}

STDMETHODIMP CFolderComponentImpl::RestoreResultView(MMC_COOKIE, PRESULT_VIEW_TYPE_INFO)
{
    return E_NOTIMPL;
}
*/

/* Folder static node implementation */

DEFINE_GUID(NODETYPE_STATICFOLDERNODE, 0xC96401CE, 0x0E17, 0x11D3, 0x88, 0x5B, 0x00, 0xC0, 0x4F, 0x72, 0xFF, 0xFF);

STDMETHODIMP CFolderSnapin::GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium)
{
    Logger::Instance() << "CFolderSnapin::GetDataHere" << Endl;
    if (pmedium == nullptr || pformatetc == nullptr)
    {
        return E_POINTER;
    }

    // Check if the medium is HGLOBAL
    if (pformatetc->tymed != TYMED::TYMED_HGLOBAL || pmedium->tymed != TYMED::TYMED_HGLOBAL)
    {
        return DV_E_TYMED;
    }

    // TODO: Cache these?
    const CLIPFORMAT nodetype    = RegisterClipboardFormat(CCF_NODETYPE);
    //const CLIPFORMAT sznodetype  = RegisterClipboardFormat(CCF_SZNODETYPE);
    const CLIPFORMAT displayname = RegisterClipboardFormat(CCF_DISPLAY_NAME);
    const CLIPFORMAT snapinclsid = RegisterClipboardFormat(CCF_SNAPIN_CLASSID);

    const void *src_ptr;
    void *dst_ptr;
    size_t cbLen; // Size in bytes of data to copy to HGLOBAL
    
    pmedium->pUnkForRelease = NULL;

    if (pformatetc->cfFormat == nodetype)
    {
        // Return the NodeType GUID
        src_ptr = reinterpret_cast<const void*>(&NODETYPE_STATICFOLDERNODE);
        cbLen = sizeof(NODETYPE_STATICFOLDERNODE);
    }
    else if (pformatetc->cfFormat == displayname)
    {
        // Return the folder display name
        src_ptr = reinterpret_cast<const void*>(m_name.c_str());
        cbLen = (m_name.size() + 1) * sizeof(std::wstring::traits_type::char_type);
    }
    else if (pformatetc->cfFormat == snapinclsid)
    {
        // Return the CLSID GUID
        src_ptr = reinterpret_cast<const void*>(&CLSID_Folder);
        cbLen = sizeof(CLSID_Folder);
    }
    else // Not any of the supported clipboard types
    {
        return DV_E_FORMATETC;
    }

    // Copy the retrieved data into the HGLOBAL
    dst_ptr = GlobalLock(pmedium->hGlobal);
    if (dst_ptr == nullptr)
    {
        Logger::Instance() << "CFolderSnapin::GetDataHere couldn't GlobalLock(HGLOBAL)" << Endl;
        return E_UNEXPECTED;
    }

    memcpy(dst_ptr, src_ptr, cbLen); // Dear god, don't crash.

    GlobalUnlock(pmedium->hGlobal);

    return S_OK;
}

STDMETHODIMP CFolderSnapin::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
    if(pformatetcIn == nullptr || pmedium == nullptr)
    {
        return E_POINTER;
    }
    WCHAR clipformat_name[MAX_PATH];
    int clipformat_name_len = GetClipboardFormatName(pformatetcIn->cfFormat, clipformat_name, MAX_PATH - 1);
    std::wstring cfname = std::wstring(clipformat_name, clipformat_name_len);
    std::string cfname_thin = std::string(cfname.begin(), cfname.end());
    Logger::Instance() << "CFolderSnapin::GetData(format: " << cfname_thin.c_str() << Endl;
    return DV_E_FORMATETC;
}
