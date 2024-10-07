#include "precomp.h"

#include "mmcndmgr_classes.h"

#include "FolderSnapin.h"

// Define the module class
class CMmcNodeMgrModule : public CComModule {};

// Required global instance of module;
CMmcNodeMgrModule _AtlModule;

BEGIN_OBJECT_MAP(ObjectMap)
    OBJECT_ENTRY(CLSID_Folder, CFolderSnapin)
END_OBJECT_MAP()

STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID fImpLoad)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _AtlModule.Init(ObjectMap, hInstance, NULL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        _AtlModule.Term();
    }
    return TRUE;
}

extern "C" HRESULT WINAPI DllCanUnloadNow()
{
    return _AtlModule.DllCanUnloadNow();
}

extern "C" HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

extern "C" HRESULT WINAPI DllRegisterServer()
{
    return _AtlModule.DllRegisterServer();
}

extern "C" HRESULT WINAPI DllUnregisterServer()
{
    return _AtlModule.DllUnregisterServer();
}