#include "AddRemoveDialog.h"

#include <vector>
#include <string>

struct SnapinInfo
{
    std::wstring clsid;
    std::wstring name;
};

std::vector<SnapinInfo> EnumerateRegisteredStandaloneSnapIns()
{
    std::vector<SnapinInfo> clsidList;
    CRegKey regKey;
    LONG lResult;

    // Open the registry key where Snap-ins are registered
    lResult = regKey.Open(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\MMC\\SnapIns"), KEY_READ);
    if (lResult != ERROR_SUCCESS)
    {
        RaiseException(0, 0, 0, NULL);
    }

    // Enumerate through the subkeys
    DWORD index = 0;
    wchar_t clsidBuffer[256];
    const DWORD cbClsidBuffer = sizeof(clsidBuffer) / sizeof(clsidBuffer[0]);
    DWORD clsidSize = cbClsidBuffer;

    while (regKey.EnumKey(index, clsidBuffer, &clsidSize) == ERROR_SUCCESS)
    {
        SnapinInfo sii;
        sii.clsid = std::wstring(clsidBuffer);
        // Construct the full path for the current CLSID key
        CRegKey regKeyStandalone;
        std::wstring snapInPath = TEXT("SOFTWARE\\Microsoft\\MMC\\SnapIns\\") + sii.clsid;

        // Open the Snap-in's key
        CRegKey regKeySnapIn;
        regKeySnapIn.Open(HKEY_LOCAL_MACHINE, snapInPath.c_str(), KEY_READ);

        // Get the NameStringIndirect
        wchar_t namestringindirect[256];
        const DWORD cbNameStringIndirect = sizeof(namestringindirect) / sizeof(namestringindirect[0]);
        lResult = RegLoadMUIString(regKeySnapIn.m_hKey, TEXT("NameStringIndirect"), namestringindirect, cbNameStringIndirect, NULL, REG_MUI_STRING_TRUNCATE, NULL);
        if (lResult == ERROR_SUCCESS) {
            sii.name = std::wstring(namestringindirect);
        }

        // Check if the "Standalone" subkey exists under this CLSID
        lResult = regKeyStandalone.Open(HKEY_LOCAL_MACHINE, (snapInPath + L"\\Standalone").c_str(), KEY_READ);
        if (lResult == ERROR_SUCCESS)
        {
            // If the "Standalone" subkey exists, add the CLSID to the list
            clsidList.push_back(sii);
        }

        clsidSize = cbClsidBuffer; // Reset buffer size
        ++index;
    }

    return clsidList;
};

LRESULT CAddRemoveDialog::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
    LVITEM lvi;
    LVCOLUMN lvc;
    // Get a list of Snap-ins registered on the computer.
    std::vector<SnapinInfo> snapins = EnumerateRegisteredStandaloneSnapIns();

    // Store handle to ListView
    m_hListView = GetDlgItem(IDL_STANDALONESNAPLIST);

    // Get the width of the ListView
    RECT lvRect;
    ::GetClientRect(m_hListView, &lvRect);

    int width = lvRect.right - lvRect.left;

    // Add the "Name" column
    lvc.mask = LVCF_SUBITEM | LVCF_WIDTH;
    lvc.cx = width;
    lvc.iSubItem = 0;
    ListView_InsertColumn(m_hListView, 0, &lvc);

    // Initialize LVITEM members that are common to all items.
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.stateMask = 0;
    lvi.iSubItem = 0;
    lvi.state = 0;

    int i = 0;
    for (const SnapinInfo &snapin : snapins)
    {
        lvi.iItem = i;
        lvi.pszText = const_cast<LPTSTR>(snapin.name.c_str());
        ListView_InsertItem(m_hListView, &lvi);
        i++;
    }

    return 1;
}

LRESULT CAddRemoveDialog::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
    EndDialog(0);
    return 0;
}