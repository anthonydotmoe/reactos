#pragma once

#include "precomp.h"

class CAddRemoveDialog : public CDialogImpl<CAddRemoveDialog>
{
public:

    enum { IDD = IDD_ADDREMOVESNAP };

    BEGIN_MSG_MAP(CAddRemoveDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&);

private:
    HWND m_hListView;
};