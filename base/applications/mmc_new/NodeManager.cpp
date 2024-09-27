#include "precomp.h"
#include "NodeManager.h"

const IID TheISnapinAbout = IID_ISnapinAbout;

/******************************************************************************
 * load_string [Internal]
 *
 * This is basically a copy of user32/resource.c's LoadStringW. Necessary to
 * avoid importing user32, which is higher level than advapi32. Helper for
 * RegLoadMUIString.
 */
static int load_string(HINSTANCE hModule, UINT resId, LPWSTR pwszBuffer, INT cMaxChars)
{
    HGLOBAL hMemory;
    HRSRC hResource;
    WCHAR* pString;
    int idxString;

    /* Negative values have to be inverted. */
    if (HIWORD(resId) == 0xffff)
        resId = (UINT)(-((INT)resId));

    /* Load the resource into memory and get a pointer to it. */
    hResource = FindResourceW(hModule, MAKEINTRESOURCEW(LOWORD(resId >> 4) + 1), (LPWSTR)RT_STRING);
    if (!hResource) return 0;
    hMemory = LoadResource(hModule, hResource);
    if (!hMemory) return 0;
    pString = static_cast<WCHAR*>(LockResource(hMemory));

    /* Strings are length-prefixed. Lowest nibble of resId is an index. */
    idxString = resId & 0xf;
    while (idxString--) pString += *pString + 1;

    /* If no buffer is given, return length of the string. */
    if (!pwszBuffer) return *pString;

    /* Else copy over the string, respecting the buffer size. */
    cMaxChars = (*pString < cMaxChars) ? *pString : (cMaxChars - 1);
    if (cMaxChars >= 0)
    {
        memcpy(pwszBuffer, pString + 1, cMaxChars * sizeof(WCHAR));
        pwszBuffer[cMaxChars] = L'\0';
    }

    return cMaxChars;
}

/************************************************************************
 *  RegLoadMUIStringW
 *
 * @implemented
 */
LONG WINAPI
MyRegLoadMUIString(
    IN HKEY hKey,
    IN LPCWSTR pszValue  OPTIONAL,
    OUT LPWSTR pszOutBuf,
    IN DWORD cbOutBuf,
    OUT LPDWORD pcbData OPTIONAL,
    IN DWORD Flags,
    IN LPCWSTR pszDirectory  OPTIONAL)
{
    DWORD dwValueType, cbData;
    LPWSTR pwszTempBuffer = NULL, pwszExpandedBuffer = NULL;
    LONG result;

    /* Parameter sanity checks. */
    if (!hKey || !pszOutBuf)
        return ERROR_INVALID_PARAMETER;

    if (pszDirectory && *pszDirectory)
    {
        //FIXME("BaseDir parameter not yet supported!\n");
        return ERROR_INVALID_PARAMETER;
    }

    /* Check for value existence and correctness of it's type, allocate a buffer and load it. */
    result = RegQueryValueExW(hKey, pszValue, NULL, &dwValueType, NULL, &cbData);
    if (result != ERROR_SUCCESS) goto cleanup;
    if (!(dwValueType == REG_SZ || dwValueType == REG_EXPAND_SZ) || !cbData)
    {
        result = ERROR_FILE_NOT_FOUND;
        goto cleanup;
    }
    pwszTempBuffer = static_cast<LPWSTR>(HeapAlloc(GetProcessHeap(), 0, cbData));
    if (!pwszTempBuffer)
    {
        result = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }
    result = RegQueryValueExW(hKey, pszValue, NULL, &dwValueType, (LPBYTE)pwszTempBuffer, &cbData);
    if (result != ERROR_SUCCESS) goto cleanup;

    /* Expand environment variables, if appropriate, or copy the original string over. */
    if (dwValueType == REG_EXPAND_SZ)
    {
        cbData = ExpandEnvironmentStringsW(pwszTempBuffer, NULL, 0) * sizeof(WCHAR);
        if (!cbData) goto cleanup;
        pwszExpandedBuffer = static_cast<LPWSTR>(HeapAlloc(GetProcessHeap(), 0, cbData));
        if (!pwszExpandedBuffer)
        {
            result = ERROR_NOT_ENOUGH_MEMORY;
            goto cleanup;
        }
        ExpandEnvironmentStringsW(pwszTempBuffer, pwszExpandedBuffer, cbData);
    }
    else
    {
        pwszExpandedBuffer = static_cast<LPWSTR>(HeapAlloc(GetProcessHeap(), 0, cbData));
        memcpy(pwszExpandedBuffer, pwszTempBuffer, cbData);
    }

    /* If the value references a resource based string, parse the value and load the string.
     * Else just copy over the original value. */
    result = ERROR_SUCCESS;
    if (*pwszExpandedBuffer != L'@') /* '@' is the prefix for resource based string entries. */
    {
        lstrcpynW(pszOutBuf, pwszExpandedBuffer, cbOutBuf / sizeof(WCHAR));
    }
    else
    {
        WCHAR* pComma = wcsrchr(pwszExpandedBuffer, L',');
        UINT uiStringId;
        HMODULE hModule;

        /* Format of the expanded value is 'path_to_dll,-resId' */
        if (!pComma || pComma[1] != L'-')
        {
            result = ERROR_BADKEY;
            goto cleanup;
        }

        uiStringId = _wtoi(pComma + 2);
        *pComma = L'\0';

        hModule = LoadLibraryExW(pwszExpandedBuffer + 1, NULL, LOAD_LIBRARY_AS_DATAFILE);
        if (!hModule || !load_string(hModule, uiStringId, pszOutBuf, cbOutBuf / sizeof(WCHAR)))
            result = ERROR_BADKEY;
        FreeLibrary(hModule);
    }

cleanup:
    HeapFree(GetProcessHeap(), 0, pwszTempBuffer);
    HeapFree(GetProcessHeap(), 0, pwszExpandedBuffer);
    return result;
}

void UpdateSnapinInfo(std::vector<SnapinInfo> &snapins);

NodeManager::NodeManager()
{
    // Retrieve all standalone snapins
    CRegKey rkSnapins;
    LRESULT lResult;

    lResult = rkSnapins.Open(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\MMC\\SnapIns"), KEY_READ);
    if (lResult != ERROR_SUCCESS)
        return; //TODO: Do something else?
    
    // Enumerate through the subkeys
    DWORD index = 0;
    wchar_t clsidBuffer[256]; // TODO: Something better please?
    const DWORD cbClsidBuffer = sizeof(clsidBuffer) / sizeof(clsidBuffer[0]);
    DWORD clsidSize = cbClsidBuffer;

    while (rkSnapins.EnumKey(index, clsidBuffer, &clsidSize) == ERROR_SUCCESS)
    {
        SnapinInfo sii;
        sii.clsid = std::wstring(clsidBuffer);
        // Construct the full path for the current CLSID key
        CRegKey rkSnapIn;
        std::wstring snapInPath = TEXT("SOFTWARE\\Microsoft\\MMC\\SnapIns\\") + sii.clsid;

        // Open the Snap-in's key
        rkSnapIn.Open(rkSnapins, clsidBuffer, KEY_READ);

        // Get the NameStringIndirect
        wchar_t namestringindirect[256];
        const DWORD cbNameStringIndirect = sizeof(namestringindirect) / sizeof(namestringindirect[0]);
        lResult = MyRegLoadMUIString(rkSnapIn.m_hKey, TEXT("NameStringIndirect"), namestringindirect, cbNameStringIndirect, NULL, 1, NULL);
        if (lResult == ERROR_SUCCESS) {
            sii.name = std::wstring(namestringindirect);
        }

        // Get the ISnapinAbout CLSID
        CLSID clsidAbout = {0};
        lResult = rkSnapIn.QueryGUIDValue(TEXT("About"), clsidAbout);
        if (lResult == ERROR_SUCCESS) {
            sii.hasAbout = TRUE;
            sii.aboutclsid = clsidAbout;
        }
        else
        {
            sii.hasAbout = FALSE;
        }

        // Check if the "Standalone" subkey exists under this CLSID
        CRegKey rkStandalone;
        lResult = rkStandalone.Open(HKEY_LOCAL_MACHINE, (snapInPath + L"\\Standalone").c_str(), KEY_READ);
        if (lResult == ERROR_SUCCESS)
        {
            // If the "Standalone" subkey exists, add the CLSID to the list
            m_colStandalone.push_back(sii);
        }
        else
        {
            // It's probably an extension, add it to extensions list
            // TODO: Assumption?
            m_colExtensions.push_back(sii);
        }

        clsidSize = cbClsidBuffer; // Reset buffer size
        ++index;
    }

    // Update the ISnapinAbout data
    UpdateSnapinInfo(m_colStandalone);
    UpdateSnapinInfo(m_colExtensions);

}

// Enumerate `snapins` and use its associated ISnapinAbout to retrieve and store
// the info.
void UpdateSnapinInfo(std::vector<SnapinInfo> &snapins)
{
    for (SnapinInfo &snap : snapins)
    {
        if (snap.hasAbout)
        {
            HRESULT hr;
            CComPtr<ISnapinAbout> pSnapinAbout;

            // ??? ATL CComPtr?
            hr = pSnapinAbout.CoCreateInstance(static_cast<const _GUID&>(snap.aboutclsid), TheISnapinAbout, NULL, CLSCTX_INPROC_SERVER);
            if (hr != S_OK)
                continue;
            
            // Get Provider
            LPOLESTR provider;
            hr = pSnapinAbout->GetProvider(&provider);
            if (hr == S_OK)
            {
                snap.provider = std::wstring(provider);
                CoTaskMemFree(provider);
            }

            // Get Description
            LPOLESTR description;
            hr = pSnapinAbout->GetSnapinDescription(&description);
            if (hr == S_OK)
            {
                snap.description = std::wstring(description);
                CoTaskMemFree(description);
            }
        }
    }
}