#include "ChildFrame.h"

LRESULT CChildFrame::OnQueryClose(UINT, WPARAM, LPARAM, BOOL &bHandled)
{
    UINT msg = MessageBox(_T("May I close this window?"), _T("Confirm"), MB_ICONQUESTION | MB_YESNO);

    // If it's OK, let DefMDIChildProc close us
    if (msg == IDYES)
        bHandled = FALSE;
    
    return 0;
}

LRESULT CChildFrame::OnCreate(UINT, WPARAM, LPARAM, BOOL)
{
    // Create the controls
    RECT rcClient;
    HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
    GetClientRect(&rcClient);

    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;

    int treeWidth = width / 5;

    m_hTreeView = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_TREEVIEW, NULL,
        WS_VISIBLE | WS_CHILD | TVS_HASLINES,
        0, 0, treeWidth, height,
        m_hWnd, (HMENU)IDC_TREEVIEW, hInst, NULL
    );

    m_hListView = CreateWindowEx(
        WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL,
        WS_VISIBLE | WS_CHILD | LVS_REPORT,
        treeWidth, 0, width - treeWidth, height,
        m_hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL
    );

    if (!m_hTreeView || !m_hListView)
    {
        // Couldn't create the window controls. Just die.
        return -1;
    }
    else
    {
        return 0; // Continue creation of the window
    }
}

void CChildFrame::OnFinalMessage(HWND)
{
    delete this;
}