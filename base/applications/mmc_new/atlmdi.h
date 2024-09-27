#pragma once

#include "precomp.h"

template <typename T> class CMdiFrameWindow;

class CMdiClientWindow;
class CMdiChildWindow;

/* Uh oh, ReactOS ATL doesn't have WM_FORWARDMSG */
// Define WM_FORWARDMSG to pass compilation
#define WM_FORWARDMSG 0x037F

// The MDI Client Window (Window hosted in the client area of the main
// application that hosts the child windows)
class CMdiClientWindow : public CWindow
{
public:
    static LPCTSTR GetWndClassName()
    {
        return _T("MDICLIENT");
    }

    HWND Create(HWND hWndParent, _U_RECT rect, LPCTSTR szWindowName = NULL,
                DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U,
                LPVOID lpCreateParam = NULL
    )
    {
        return CWindow::Create(GetWndClassName(), hWndParent, rect, szWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    }

    BOOL Translate(MSG *pMsg, HACCEL hAccel = 0)
    {
        HWND hWnd = MdiGetActive();
        if (!hWnd)
            return FALSE;

        return SendMessage(hWnd, WM_FORWARDMSG, 0, reinterpret_cast<LPARAM>(pMsg));
    }

    HWND MdiGetActive()
    {
        HWND hWndChild = reinterpret_cast<HWND>(SendMessage(WM_MDIGETACTIVE));
        return hWndChild;
    }

    void MdiDestroy(HWND hWndChild)
    {
        SendMessage(WM_MDIDESTROY, reinterpret_cast<WPARAM>(hWndChild));
    }

    void MdiTile(UINT flags = MDITILE_VERTICAL | MDITILE_SKIPDISABLED)
    {
        SendMessage(WM_MDICASCADE, flags);
    }

    void MdiCascade(UINT flags = MDITILE_SKIPDISABLED)
    {
        SendMessage(WM_MDICASCADE, flags);
    }

    void MdiArrange()
    {
        SendMessage(WM_MDIICONARRANGE);
    }

    void MdiCloseAll()
    {
        EnumChildWindows(m_hWnd, CloseEnumProc, 0);
    }

    void MdiMaximize(HWND hWndChild)
    {
        SendMessage(WM_MDIMAXIMIZE, reinterpret_cast<WPARAM>(hWndChild));
    }

    void MdiNext(HWND hWndChild = 0)
    {
        SendMessage(WM_MDINEXT, reinterpret_cast<WPARAM>(hWndChild ? hWndChild : MdiGetActive()), TRUE);
    }

    void MdiPrevious(HWND hWndChild = 0)
    {
        SendMessage(WM_MDINEXT, reinterpret_cast<WPARAM>(hWndChild ? hWndChild : MdiGetActive()), FALSE);
    }

    void MdiRefreshMenu()
    {
        SendMessage(WM_MDIREFRESHMENU);
    }

    void MdiRestore(HWND hWndChild)
    {
        SendMessage(WM_MDIRESTORE);
    }

    void MdiSetMenu(HMENU hMenuFrame, HMENU hMenuWindow = 0)
    {
        SendMessage(WM_MDISETMENU, reinterpret_cast<WPARAM>(hMenuFrame), reinterpret_cast<LPARAM>(hMenuWindow));
    }

private:
    static BOOL CALLBACK CloseEnumProc(HWND hWnd, LPARAM lParam)
    {
        if (::GetWindow(hWnd, GW_OWNER))
            return TRUE;
        
        ::SendMessage(::GetParent(hWnd), WM_MDIRESTORE, reinterpret_cast<WPARAM>(hWnd), 0);
        if (!::SendMessage(hWnd, WM_QUERYENDSESSION, 0, 0))
            return TRUE;
        ::SendMessage(::GetParent(hWnd), WM_MDIDESTROY, reinterpret_cast<WPARAM>(hWnd), 0);

        return TRUE;
    }
};

// The MDI Frame Window (Main Application Window)
typedef CFrameWinTraits CMdiFrameWinTraits;

template <typename T, typename Base = CWindow, typename Traits = CMdiFrameWinTraits>
class CMdiFrameWindowImpl : public CWindowImpl<T, Base, Traits>
{
public:
    CMdiFrameWindowImpl() : m_hMenu(0) {}

DECLARE_WND_CLASS_EX(0, CS_HREDRAW | CS_VREDRAW, COLOR_APPWORKSPACE)
BEGIN_MSG_MAP(CMdiFrameWindowImpl)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryClose)
    MESSAGE_HANDLER(WM_CLOSE, OnQueryClose)
    MESSAGE_HANDLER(WM_MDISETMENU, OnMdiSetMenu)
    COMMAND_ID_HANDLER(IDM_FILE_EXIT, OnAppExit)
    //COMMAND_ID_HANDLER(ID_WINDOW_TILE_VERT, OnWindowTile)
    COMMAND_ID_HANDLER(IDM_WINDOW_TILEHORZ, OnWindowTile)
    COMMAND_ID_HANDLER(IDM_WINDOW_CASCADE, OnWindowCascade)
    COMMAND_ID_HANDLER(IDM_WINDOW_ARRANGEICONS, OnWindowArrange)
    // Let the child handle it, and then pass it along to DefFrameProc
    if (uMsg == WM_COMMAND)
    {
        bHandled = FALSE;
        HWND hWndChild = m_wndClient.MdiGetActive();
        if (hWndChild)
            lResult = SendMessage(hWndChild, uMsg, wParam, lParam);
        if (bHandled)
            return TRUE;
    }
END_MSG_MAP()

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
    {
        // Cache our menu
        m_hMenu = GetMenu();

        // Create the client window
        CLIENTCREATESTRUCT ccs;
        ccs.hWindowMenu = 0;
        ccs.idFirstChild = IDM_MDI_FIRSTCHILD;

        RECT rect = {0};
        HWND hWndClient = m_wndClient.Create(
            m_hWnd, rect, 0,
            WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
            WS_EX_CLIENTEDGE, IDM_MDI_CLIENT, reinterpret_cast<LPVOID>(&ccs)
        );

        return (hWndClient ? 0 : -1);
    }

    LRESULT OnQueryClose(UINT, WPARAM, LPARAM, BOOL &bHandled)
    {
        // Try to close all the children
        m_wndClient.MdiCloseAll();

        // If we closed all the children, let DefFrameProc close us
        if (m_wndClient.GetWindow(GW_CHILD) == 0)
            bHandled = FALSE;
        
        return 0;
    }

    LRESULT OnWindowClose(WORD, WORD, HWND, BOOL&)
    {
        // Close the active window
        CWindow wndChild = m_wndClient.MdiGetActive();
        if (wndChild.m_hWnd && wndChild.SendMessage(WM_QUERYENDSESSION))
        {
            m_wndClient.MdiDestroy(wndChild);
        }

        return 0;
    }

    LRESULT OnMdiSetMenu(UINT, WPARAM, LPARAM, BOOL&)
    {
        // When an MDI child window is going away, it will ask
        // us to restore our menu
        m_wndClient.MdiSetMenu(m_hMenu);
        return 0;
    }

    LRESULT OnAppExit(WORD, WORD, HWND, BOOL&)
    {
        SendMessage(WM_CLOSE);
        return 0;
    }

    LRESULT OnWindowTile(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiTile(MDITILE_HORIZONTAL | MDITILE_SKIPDISABLED);
        return 0;
    }

    LRESULT OnWindowCascade(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiCascade();
        return 0;
    }

    LRESULT OnWindowArrange(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiArrange();
        return 0;
    }

    LRESULT OnWindowCloseAll(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiCloseAll();
        return 0;
    }

    LRESULT OnWindowNext(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiNext();
        return 0;
    }

    LRESULT OnWindowPrevious(WORD, WORD, HWND, BOOL&)
    {
        m_wndClient.MdiPrevious();
        return 0;
    }

    BOOL Translate(MSG *pMsg, HACCEL hAccel = 0)
    {
        if (::TranslateMDISysAccel(m_wndClient, pMsg) || m_wndClient.Translate(pMsg) )
        {
            return TRUE;
        }

        return FALSE;
    }

    // Hook up our WndProc instead of the base's
    virtual WNDPROC GetWindowProc()
    {
        return MdiFrameBaseWindowProc;
    }

    // Provide this function merely to bind to our DefWindowProc instead of CWindowImplBaseT
    static LRESULT CALLBACK MdiFrameBaseWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // NOTE: This is the only changed line of code.
        //       The rest is copied from CWindowImplBaseT::WindowProc
        CMdiFrameWindowImpl* pThis = reinterpret_cast<CMdiFrameWindowImpl*>(hWnd);

        _ATL_MSG msg(pThis->m_hWnd, uMsg, wParam, lParam);
        LRESULT lResult;
        const _ATL_MSG *previousMessage;
        BOOL handled;
        LONG_PTR saveWindowProc;
 
        ATLASSERT(pThis != NULL);
        if (pThis == NULL)
            return 0;
 
        ATLASSERT((pThis->m_dwState & WINSTATE_DESTROYED) == 0);
        ATLASSERT(pThis->m_hWnd != NULL);
        if ((pThis->m_dwState & WINSTATE_DESTROYED) != 0 || pThis->m_hWnd == NULL)
            return 0;
 
        hWnd = pThis->m_hWnd;
        previousMessage = pThis->m_pCurrentMsg;
        pThis->m_pCurrentMsg = &msg;
 
        handled = pThis->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, 0);
        ATLASSERT(pThis->m_pCurrentMsg == &msg);
 
        if (handled == FALSE)
        {
            if (uMsg == WM_NCDESTROY)
            {
                saveWindowProc = ::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
                lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
                if (pThis->m_pfnSuperWindowProc != ::DefWindowProc && saveWindowProc == ::GetWindowLongPtr(hWnd, GWLP_WNDPROC))
                    ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(pThis->m_pfnSuperWindowProc));
                pThis->m_dwState |= WINSTATE_DESTROYED;
            }
            else
            {
                lResult = pThis->DefWindowProc(uMsg, wParam, lParam);
            }
        }
        ATLASSERT(pThis->m_pCurrentMsg == &msg);
        pThis->m_pCurrentMsg = previousMessage;
        if (previousMessage == NULL && (pThis->m_dwState & WINSTATE_DESTROYED) != 0)
        {
            pThis->m_dwState &= ~WINSTATE_DESTROYED;
            pThis->m_hWnd = NULL;
            pThis->OnFinalMessage(hWnd);
        }
        return lResult;
    }

    LRESULT DefWindowProc()
    {
        if (!m_pCurrentMsg) return 0;
        return DefWindowProc(m_pCurrentMsg->message, m_pCurrentMsg->wParam, m_pCurrentMsg->lParam);
    }

    LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        return DefFrameProc(m_hWnd, m_wndClient.m_hWnd, message, wParam, lParam);
    }

protected:
    CMdiClientWindow m_wndClient;
    HMENU m_hMenu;
};

// Now, the Child Window, the actual document of the MDI
template <typename T, typename TBase = CWindow, typename TWinTraits = CMDIChildWinTraits>
class CMdiChildWindowImpl : public CWindowImpl<CMdiChildWindowImpl<T>, TBase, TWinTraits>
{
public:

typedef CMdiChildWindowImpl<T, TBase, TWinTraits> thisClass;
BEGIN_MSG_MAP(thisClass)
    MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
    MESSAGE_HANDLER(WM_MDIACTIVATE, OnMdiActivate)
END_MSG_MAP()

    CMdiChildWindowImpl()
    :   m_hWndClient(0),
        m_hMenu(0)
    {
        // Route unhandled messages to the MDI Child WndProc
        m_pfnSuperWindowProc = DefMDIChildProc;
    }

    HWND Create(HWND hWndMdiClient, _U_RECT rect, LPCTSTR szWindowName = 0,
                DWORD dwStyle = 0, HMENU hMenu = 0, LPARAM lParam = 0
    )
    {
        m_hWndClient = hWndMdiClient;
        SetMenu(hMenu);

        // We need the class name to fill in the struct, so register it if it
        // hasn't already been registered.
        ATOM atom = T::GetWndClassInfo().Register(&m_pfnSuperWindowProc);

        // Create a thunk
        _AtlWinModule.AddCreateWndData(&m_thunk.cd, this);

        MDICREATESTRUCT mcs = {0};
        mcs.szClass = reinterpret_cast<LPCTSTR>(MAKELONG(atom, 0));
        mcs.szTitle = szWindowName;
        mcs.hOwner  = _AtlBaseModule.GetModuleInstance();
        mcs.x       = rect.m_lpRect->left;
        mcs.y       = rect.m_lpRect->top;
        mcs.cx      = rect.m_lpRect->right - rect.m_lpRect->left;
        mcs.cy      = rect.m_lpRect->bottom - rect.m_lpRect->top;
        mcs.lParam  = lParam;
        mcs.style   = dwStyle;

        HWND ret;
        ret = reinterpret_cast<HWND>(::SendMessage(m_hWndClient, WM_MDICREATE, 0, reinterpret_cast<LPARAM>(&mcs)));

        return ret;
    }

    LRESULT OnForwardMsg(UINT, WPARAM, LPARAM lParam, BOOL&)
    {
        MSG *pMsg = reinterpret_cast<MSG*>(lParam);
        _ASSERTE(pMsg);
        return static_cast<T*>(this)->Translate(pMsg);
    }

    BOOL Translate(MSG *pMsg, HACCEL hAccel = 0)
    {
        return FALSE;
    }

    LRESULT OnMdiActivate(UINT, WPARAM, LPARAM lParam, BOOL&)
    {
        // If we're gaining focus, ask the client to show our menu
        if ((reinterpret_cast<HWND>(lParam) == m_hWnd) && m_hMenu)
        {
            ::SendMessage(m_hWndClient, WM_MDISETMENU, reinterpret_cast<WPARAM>(m_hMenu), reinterpret_cast<LPARAM>(m_hMenuWindow));
        }
        // If we're losing focus, ask the frame to restore it's own menu
        else
        {
            ::SendMessage(::GetParent(m_hWndClient), WM_MDISETMENU, 0, 0);
        }

        ::DrawMenuBar(::GetParent(m_hWndClient));
        return 0;
    }

    HMENU GetMenu()
    {
        return m_hMenu;
    }

    void SetMenu(HMENU hMenu)
    {
        m_hMenu = hMenu;

        // Find the &Window menu so the MDI Client can list all the windows
        if (m_hMenu)
        {
            // The &Window menu is the second to the last menu item (&Help is last)
            int nWindowMenu = GetMenuItemCount(m_hMenu) - 2;
            if (nWindowMenu < 0)
                return;

            const TCHAR szWindow[] = _T("&Window");
            const size_t cchWindow = sizeof(szWindow)/sizeof(*szWindow) - 1;
            TCHAR szMenuName[cchWindow+1];
            int ret = GetMenuString(m_hMenu, nWindowMenu, szMenuName, cchWindow + 1, MF_BYPOSITION);
            if (!ret)
                return;
            
            if (CompareString(LOCALE_SYSTEM_DEFAULT, 0, szWindow, cchWindow, szMenuName, -1) == CSTR_EQUAL)
            {
                m_hMenuWindow = GetSubMenu(m_hMenu, nWindowMenu);
            }
        }
    }
    
private:
    HWND  m_hWndClient;
    HMENU m_hMenu;
    HMENU m_hMenuWindow;
};