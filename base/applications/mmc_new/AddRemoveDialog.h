#pragma once

#include "precomp.h"

class CAddRemoveStandalonePropSheet : public CPropertyPageImpl<CAddRemoveStandalonePropSheet>
{
public:

    enum { IDD = IDD_ADDREMOVESTANDALONESHEET };

    BEGIN_MSG_MAP(CAddRemoveStandalonePropSheet)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        CHAIN_MSG_MAP(CPropertyPageImpl<CAddRemoveStandalonePropSheet>)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);

private:
    CListView m_ListView;
};

class CAddRemoveExtensionsPropSheet : public CPropertyPageImpl<CAddRemoveExtensionsPropSheet>
{
public:

    enum { IDD = IDD_ADDREMOVEEXTENSIONSHEET };

    BEGIN_MSG_MAP(CAddRemoveExtensionsPropSheet)
        CHAIN_MSG_MAP(CPropertyPageImpl<CAddRemoveExtensionsPropSheet>)
    END_MSG_MAP()

private:
    HWND m_hListView;
};