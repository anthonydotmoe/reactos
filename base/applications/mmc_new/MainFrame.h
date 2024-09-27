#pragma once

#include "precomp.h"

#include "atlmdi.h"

class CMainFrame : public CMdiFrameWindowImpl<CMainFrame>
{
public:
    CMainFrame();

typedef CMdiFrameWindowImpl<CMainFrame> baseClass;

DECLARE_WND_CLASS(_T("MMCMainFrame"))

BEGIN_MSG_MAP(CMainFrame)
    COMMAND_ID_HANDLER(IDM_FILE_NEW, OnFileNew)
    COMMAND_ID_HANDLER(IDM_HELP_ABOUT, OnHelpAbout)
    COMMAND_ID_HANDLER(IDM_FILE_ADDREMOVESNAP, OnAddRemoveSnap)

    // All the rest of standard Window menu items are defined in ((()))
    // and handled in CMdiFrameWindowImpl. However, these four don't have
    // standard menu item IDs and therefore need to be forwarded manually.
    COMMAND_ID_HANDLER(IDM_WINDOW_CLOSE, baseClass::OnWindowClose)
    COMMAND_ID_HANDLER(IDM_WINDOW_CLOSEALL, baseClass::OnWindowCloseAll)
    COMMAND_ID_HANDLER(IDM_WINDOW_NEXT, baseClass::OnWindowNext)
    COMMAND_ID_HANDLER(IDM_WINDOW_PREVIOUS, baseClass::OnWindowPrevious)
    CHAIN_MSG_MAP(baseClass)
END_MSG_MAP()

    LRESULT OnFileNew(WORD, WORD, HWND, BOOL&);
    LRESULT OnAddRemoveSnap(WORD, WORD, HWND, BOOL&);

    LRESULT OnHelpAbout(WORD, WORD, HWND, BOOL&);

    void OnFinalMessage(HWND);
};