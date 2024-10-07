#pragma once

#include "Logger.h"
#include "resource.h"
#include "precomp.h"

static HRESULT WINAPI log_stuff(void *pv, REFIID iid, LPVOID *ppv, DWORD_PTR dw)
{
    std::wstring g2s(REFIID iid);
    std::wstring output_str = g2s(iid);
    std::string str(output_str.begin(), output_str.end()); //TODO: Not very kosher...
    Logger::Instance() << "CFolderSnapin::QueryInterface(" << str << ")" << Endl;
    return E_NOINTERFACE;
}

class CFolderSnapin;

class ATL_NO_VTABLE CFolderComponentImpl :
    public IComponent,
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CFolderComponentImpl>
{
public:

DECLARE_NO_REGISTRY()
DECLARE_NOT_AGGREGATABLE(CFolderComponentImpl)

BEGIN_COM_MAP(CFolderComponentImpl)
    COM_INTERFACE_ENTRY_IID(IID_IComponent, IComponent)
END_COM_MAP()

    // IComponent Methods
    STDMETHOD(Initialize)(LPCONSOLE lpConsole);
    STDMETHOD(Notify)(LPDATAOBJECT, MMC_NOTIFY_TYPE event, LPARAM, LPARAM);
    STDMETHOD(Destroy)(MMC_COOKIE);
    STDMETHOD(QueryDataObject)(MMC_COOKIE cookie, DATA_OBJECT_TYPES type, LPDATAOBJECT *ppDataObject);
    STDMETHOD(GetResultViewType)(MMC_COOKIE cookie, LPOLESTR* ppViewType, long *pViewOptions);
    STDMETHOD(GetDisplayInfo)(RESULTDATAITEM*);
    STDMETHOD(CompareObjects)(LPDATAOBJECT, LPDATAOBJECT);

    // IComponent2 Methods
    //STDMETHOD(QueryDispatch)(MMC_COOKIE, DATA_OBJECT_TYPES, LPDISPATCH*);
    //STDMETHOD(GetResultViewType2)(MMC_COOKIE, PRESULT_VIEW_TYPE_INFO);
    //STDMETHOD(RestoreResultView)(MMC_COOKIE, PRESULT_VIEW_TYPE_INFO);

    // Parent Setter
    void SetSnapin(CFolderSnapin *pSnapin)
    {
        m_pSnapin = pSnapin;
    }

private:
    CFolderSnapin *m_pSnapin;
};

class ATL_NO_VTABLE CFolderSnapin :
    public ISnapinAbout,
    public IComponentData,
    public IDataObject,
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CFolderSnapin, &CLSID_Folder>
{

public:
    //TODO: Localize
    CFolderSnapin(): m_name(L"Folder") {}
    
    // ISnapinAbout Methods
    STDMETHOD(GetSnapinDescription)(LPOLESTR *lpDescription);
    STDMETHOD(GetProvider)(LPOLESTR *lpName);
    STDMETHOD(GetSnapinVersion)(LPOLESTR *lpVersion);
    STDMETHOD(GetSnapinImage)(HICON *hAppIcon);
    STDMETHOD(GetStaticFolderImage)(HBITMAP *hSmallImage, HBITMAP *hSmallImageOpen, HBITMAP *hLargeImage, COLORREF *cMask);

    // IComponentData Methods
    STDMETHOD(Initialize)(LPUNKNOWN lpUnknown);
    STDMETHOD(CreateComponent)(LPCOMPONENT *ppComponent);
    STDMETHOD(Notify)(LPDATAOBJECT, MMC_NOTIFY_TYPE, LPARAM, LPARAM);
    STDMETHOD(Destroy)();
    STDMETHOD(QueryDataObject)(MMC_COOKIE cookie, DATA_OBJECT_TYPES type, LPDATAOBJECT *ppDataObject);
    STDMETHOD(GetDisplayInfo)(SCOPEDATAITEM *pScopeDataItem);
    STDMETHOD(CompareObjects)(LPDATAOBJECT, LPDATAOBJECT);

    // IDataObject Methods
    STDMETHOD(DAdvise)(FORMATETC*, DWORD, IAdviseSink*, DWORD*) { return OLE_E_ADVISENOTSUPPORTED; }
    STDMETHOD(DUnadvise)(DWORD)                                 { return OLE_E_ADVISENOTSUPPORTED; }
    STDMETHOD(EnumDAdvise)(IEnumSTATDATA**)                     { return OLE_E_ADVISENOTSUPPORTED; }
    STDMETHOD(EnumFormatEtc)(DWORD, IEnumFORMATETC**)           { return OLE_S_USEREG; }
    STDMETHOD(GetCanonicalFormatEtc)(FORMATETC*, FORMATETC *pFormatEtcOut)
    {
        if (pFormatEtcOut == nullptr)
        {
            return E_POINTER;
        }
        // We have to set this field to NULL
        pFormatEtcOut->ptd = NULL;
        return E_NOTIMPL;
    }
    STDMETHOD(GetData)(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
    STDMETHOD(GetDataHere)(FORMATETC *pformatetc, STGMEDIUM *pmedium);
    STDMETHOD(QueryGetData)(FORMATETC *pformatetc) { return E_NOTIMPL; }
    STDMETHOD(SetData)(FORMATETC*, STGMEDIUM*, BOOL) { return E_NOTIMPL; }

DECLARE_REGISTRY_RESOURCEID(IDR_CLASSESREG)
DECLARE_NOT_AGGREGATABLE(CFolderSnapin)

BEGIN_COM_MAP(CFolderSnapin)
    COM_INTERFACE_ENTRY_IID(IID_ISnapinAbout, ISnapinAbout)
    COM_INTERFACE_ENTRY_IID(IID_IComponentData, IComponentData)
    COM_INTERFACE_ENTRY_IID(IID_IDataObject, IDataObject)
    COM_INTERFACE_ENTRY_FUNC_BLIND(0, log_stuff)
END_COM_MAP()

private:

    

    CComPtr<IConsole> m_pConsole;
    CComPtr<IConsoleNameSpace> m_pConsoleNameSpace;
    std::wstring m_name;
};