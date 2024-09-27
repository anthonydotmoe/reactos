#include "AddRemoveDialog.h"

#include <vector>
#include <string>

LRESULT CAddRemoveStandalonePropSheet::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
    LVITEM lvi;
    // TODO: Get a list of Snap-ins registered on the computer.

    // Store handle to ListView
    m_ListView.Attach(GetDlgItem(IDL_STANDALONESNAPLIST));

    // Get the width of the ListView
    RECT lvRect;
    m_ListView.GetClientRect(&lvRect);

    // Minus 17 to avoid making a scrollbar... There's probably a better way.
    int width = lvRect.right - lvRect.left - 17;

    // Add the "Name" column
    m_ListView.InsertColumn(0, TEXT(""), 0, width, 0);

    // Add the sorry item
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.stateMask = 0;
    lvi.iSubItem = 0;
    lvi.state = 0;
    lvi.iItem = 0;
    lvi.pszText = TEXT("Sorry...");
    m_ListView.InsertItem(&lvi);

    return 1;
}

LRESULT CAddRemoveStandalonePropSheet::OnClose(UINT, WPARAM, LPARAM, BOOL&)
{
    EndDialog(0);
    return 0;
}