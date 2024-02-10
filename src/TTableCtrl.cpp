#include "TTableCtrl.h"

const UINT TTableCtrl::K_CENTER = LVCFMT_CENTER;
const UINT TTableCtrl::K_RIGHT = LVCFMT_RIGHT;
const UINT TTableCtrl::K_LEFT = LVCFMT_LEFT;

TTableCtrl::TTableCtrl()
{
    attr.className = WC_LISTVIEW;
    attr.style |= LVS_REPORT /*| LVS_EDITLABELS*/;
    attr.styleEx = WS_EX_CLIENTEDGE;

    colIdx = 0;
}

void TTableCtrl::create(HWND hParent)
{
    TControl::create(hParent);
    ListView_SetExtendedListViewStyleEx(hWnd, LVS_EX_FULLROWSELECT,
                                        LVS_EX_FULLROWSELECT);
}

UINT TTableCtrl::getItemCount()
{
    return ListView_GetItemCount(hWnd);
}

void TTableCtrl::addColumn(LPSTR strColName, int colWidth, int alignment)
{
    LVCOLUMN col;

    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    col.fmt = alignment;
    col.cx = colWidth;
    col.pszText = strColName;
    col.cchTextMax = strlen(strColName);

    ListView_InsertColumn(hWnd, colIdx++, (LPARAM)&col);
}

void TTableCtrl::addItem(TTableItem *pItem)
{
    LVITEM item;
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.pszText = LPSTR_TEXTCALLBACK;
    item.lParam = (LPARAM)pItem;
    item.iItem = getItemCount();
    item.iSubItem = 0;

    ListView_InsertItem(hWnd, &item);
}

TTableItem *TTableCtrl::getItemAt(UINT idx)
{
    LVITEM item;
    item.mask = LVIF_PARAM;
    item.iItem = idx;
    ListView_GetItem(hWnd, &item);
    return (TTableItem *)item.lParam;
}

void TTableCtrl::onNotify(LPNMHDR lpHeader)
{
    if (lpHeader->code == LVN_GETDISPINFO)
    {
        NMLVDISPINFO *plvdi = plvdi = (NMLVDISPINFO *)lpHeader;

        TTableItem *pItem = getItemAt(plvdi->item.iItem);

        if (pItem != NULL)
        {
            plvdi->item.pszText = pItem->getColumnData(plvdi->item.iSubItem);
        }
    }
}

int TTableCtrl::getSelIndex()
{
    return ListView_GetSelectionMark(hWnd);
}

void TTableCtrl::removeItemAt(int index)
{
    TTableItem *pItem = getItemAt(index);
    if (pItem != NULL)
    {
        delete pItem;
    }

    ListView_DeleteItem(hWnd, index);
}

void TTableCtrl::removeAllItems()
{
    int count = getItemCount();
    for (int i = 0; i < count; i++) {
        TTableItem *pItem = getItemAt(i);
        delete pItem;
    }

    ListView_DeleteAllItems(hWnd);
}