#pragma once

#include "precomp.h"
#include "NodeManager.h"

class CAddRemoveStandalonePropSheet : public CPropertyPageImpl<CAddRemoveStandalonePropSheet>
{
public:

    CAddRemoveStandalonePropSheet(NodeManager &refNodeMgr):
        m_refNodeMgr(refNodeMgr)
        {}

    enum { IDD = IDD_ADDREMOVESTANDALONESHEET };

    BEGIN_MSG_MAP(CAddRemoveStandalonePropSheet)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_ID_HANDLER(IDC_ADDSTANDALONEBUTTON, OnAddStandaloneButton)
        CHAIN_MSG_MAP(CPropertyPageImpl<CAddRemoveStandalonePropSheet>)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);

    LRESULT OnAddStandaloneButton(WORD, WORD, HWND, BOOL&);

private:
    CListView m_ListView;
    NodeManager &m_refNodeMgr;
};

class CAddRemoveExtensionsPropSheet : public CPropertyPageImpl<CAddRemoveExtensionsPropSheet>
{
public:

    CAddRemoveExtensionsPropSheet(NodeManager &refNodeMgr):
        m_refNodeMgr(refNodeMgr)
        {}

    enum { IDD = IDD_ADDREMOVEEXTENSIONSHEET };

    BEGIN_MSG_MAP(CAddRemoveExtensionsPropSheet)
        CHAIN_MSG_MAP(CPropertyPageImpl<CAddRemoveExtensionsPropSheet>)
    END_MSG_MAP()

private:
    HWND m_hListView;
    NodeManager &m_refNodeMgr;
};