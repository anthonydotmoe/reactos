#include "MainFrame.h"

#include "ChildFrame.h"
#include "AddRemoveDialog.h"

CMainFrame::CMainFrame()
{
    // Initialize window info
    CWndClassInfo &wc = GetWndClassInfo();
    if (!wc.m_atom)
    {
        wc.m_wc.hIcon = LoadIcon(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDI_MAINAPP));
        wc.m_wc.hIconSm = reinterpret_cast<HICON>(
            ::LoadImage(
                _AtlBaseModule.GetResourceInstance(),
                MAKEINTRESOURCE(IDI_MAINAPP),
                IMAGE_ICON,
                16, 16,
                LR_DEFAULTCOLOR
            )
        );
    }
}

LRESULT CMainFrame::OnFileNew(WORD, WORD, HWND, BOOL&)
{
    // Create a new child window
    static HMENU hMenu = LoadMenu(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDM_CONSOLE_LARGE));
    CChildFrame *pChild = new CChildFrame;
    pChild->Create(m_wndClient, CWindow::rcDefault, TEXT(""), 0, hMenu);
    return 0;
}

// Helper laod string function
std::wstring LoadStringFromResource(HINSTANCE hInstance, UINT uID)
{
    // Pointer to the first character of the string
    const wchar_t *pchStringBegin = nullptr;

    const int cchStringLength = ::LoadStringW(
        hInstance,
        uID,
        reinterpret_cast<PWSTR>(&pchStringBegin),
        0
    );
    ATLASSERT(cchStringLength >= 0);
    if (cchStringLength > 0)
    {
        // Success
        return std::wstring(pchStringBegin, cchStringLength);
    }
    else
    {
        // Failure: throw?
        AtlThrow(E_FAIL);
        return NULL;
    }
}

LRESULT CMainFrame::OnHelpAbout(WORD, WORD, HWND, BOOL&)
{
    //if (AllocAndLoadString(&lpTitle, hAppInstance, IDS_APPTITLE))
    HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
    std::wstring appName = LoadStringFromResource(hInst, IDS_APPTITLE);
    ShellAboutW(m_hWnd, appName.c_str(), NULL, LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAINAPP)));
    return 0;
}

LRESULT CMainFrame::OnAddRemoveSnap(WORD, WORD, HWND, BOOL&)
{
    // Create Modal add/remove
    CAddRemoveStandalonePropSheet psStandalone(m_NodeManager);
    CAddRemoveExtensionsPropSheet psExtensions(m_NodeManager);
    HPROPSHEETPAGE hpsp[2] = { psStandalone.Create(), psExtensions.Create() };

    PROPSHEETHEADER psh = {};
    psh.dwSize = sizeof(psh);
    psh.dwFlags = PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP;
    psh.hInstance = _AtlBaseModule.GetResourceInstance();
    psh.pszCaption = TEXT("Add/Remove Snap-in");
    psh.nPages = _countof(hpsp);
    psh.phpage = hpsp;

    PropertySheetW(&psh);

    return 0;
}

void CMainFrame::OnFinalMessage(HWND)
{
    PostQuitMessage(0);
}
