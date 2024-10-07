#include "precomp.h"
#include "AddStandalone.h"

LRESULT CAddStandalone::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
    LVITEM lvi;

    // Store handle to ListView
    m_ListView.Attach(GetDlgItem(IDL_AVAILABLESTANDALONE));
    m_ListView.SetImageList(m_refNodeMgr.m_ilSmNodes, LVSIL_SMALL);

    // Get the width of the ListView
    RECT lvRect;
    m_ListView.GetClientRect(&lvRect);

    // Minus 17 to avoid making a scrollbar... There's probably a better way.
    int width = lvRect.right - lvRect.left - 17;
    int col1Width = width / 5 * 3;
    int col2Width = width - col1Width;

    // Add the columns
    m_ListView.InsertColumn(0, TEXT("Snap-in"), LVCFMT_IMAGE, col1Width, 0);
    m_ListView.InsertColumn(1, TEXT("Vendor"), 0, col2Width, 1);

    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvi.stateMask = 0;
    lvi.state = 0;

    int i = 0; // iItem counter
    for (const SnapinInfo &snap : m_refNodeMgr.m_colStandalone)
    {
        int thisItemIndex = 0;
        lvi.iItem = i++;
        
        // Insert Snap-in
        lvi.iSubItem = 0;
        lvi.iImage = snap.iSmallImage;
        lvi.pszText = const_cast<LPTSTR>(snap.name.c_str());
        
        // Use lParam to store a pointer to the SnapinInfo struct `const SnapinInfo *`
        lvi.lParam = reinterpret_cast<LPARAM>(&snap);

        thisItemIndex = m_ListView.InsertItem(&lvi);

        // Insert Vendor
        m_ListView.SetItemText(thisItemIndex, 1, snap.provider.c_str());
    }

    return 1;
}

LRESULT CAddStandalone::OnListViewItemActivate(int, LPNMHDR, BOOL&)
{
    // An item is selected in the ListView: Update the Description.
    LVITEM lvi;
    BOOL ret;
    lvi.mask = LVIF_PARAM;
    ret = m_ListView.GetSelectedItem(&lvi);
    if (lvi.lParam == NULL || ret == FALSE)
    {
        // If the pointer is invalid, just return.
        return 0;
    }

    LPCTSTR desc = reinterpret_cast<const SnapinInfo*>(lvi.lParam)->description.c_str();

    SetDlgItemTextW(IDC_SELECTEDSNAPINDESC, desc);

    return 0;
}

LRESULT CAddStandalone::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
    EndDialog(0);
    return 0;
}