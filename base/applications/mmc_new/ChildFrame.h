#pragma once

#include "precomp.h"

#include "atlmdi.h"

class CChildFrame : public CMdiChildWindowImpl<CChildFrame>
{
public:
    CChildFrame() {}

DECLARE_WND_CLASS(_T("MMCChildFrm"))

BEGIN_MSG_MAP(CChildFrame)
    MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryClose)
    MESSAGE_HANDLER(WM_CLOSE, OnQueryClose)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    CHAIN_MSG_MAP(CMdiChildWindowImpl<CChildFrame>)
END_MSG_MAP()

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL);
    LRESULT OnQueryClose(UINT, WPARAM, LPARAM, BOOL &bHandled);

    void OnFinalMessage(HWND);

private:
    HWND m_hTreeView;
    HWND m_hListView;
    int m_nListViewWidth;
};