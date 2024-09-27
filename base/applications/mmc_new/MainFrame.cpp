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

LRESULT CMainFrame::OnHelpAbout(WORD, WORD, HWND, BOOL&)
{
    MessageBox(TEXT("ReactOS Management Console 1.0"), TEXT("About ReactOS Management Console"));
    return 0;
}

LRESULT CMainFrame::OnAddRemoveSnap(WORD, WORD, HWND, BOOL&)
{
    // Create Modal add/remove
    CAddRemoveDialog addDlg;
    addDlg.DoModal();    

    return 0;
}

void CMainFrame::OnFinalMessage(HWND)
{
    PostQuitMessage(0);
}
