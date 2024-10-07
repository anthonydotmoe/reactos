#pragma once

#include "precomp.h"

#include "NodeManager.h"

class CAddStandalone : public CDialogImpl<CAddStandalone>
{
public:

    CAddStandalone(NodeManager &refNodeMgr) : m_refNodeMgr(refNodeMgr) {}

    enum { IDD = IDD_ADDSTANDALONE };

    BEGIN_MSG_MAP(CAddStandalone)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        NOTIFY_HANDLER(IDL_AVAILABLESTANDALONE, LVN_ITEMCHANGED, OnListViewItemActivate)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnListViewItemActivate(int, LPNMHDR, BOOL&);

private:
    
    NodeManager &m_refNodeMgr;
    CListView m_ListView;
};