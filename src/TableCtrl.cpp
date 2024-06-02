#include "TableCtrl.h"

const UINT TableCtrl::K_CENTER = LVCFMT_CENTER;
const UINT TableCtrl::K_RIGHT = LVCFMT_RIGHT;
const UINT TableCtrl::K_LEFT = LVCFMT_LEFT;

void TableItem::update()
{
    pTableCtrl->updateItem(this);
}

TableCtrl::TableCtrl()
{
    attr.className = WC_LISTVIEW;
    attr.style |= LVS_REPORT /*| LVS_EDILabelS*/;
    attr.styleEx = WS_EX_CLIENTEDGE;

    colIdx = 0;
}

void TableCtrl::create(HWND hParent)
{
    Control::create(hParent);
    ListView_SetExtendedListViewStyleEx(hWnd, LVS_EX_FULLROWSELECT,
                                        LVS_EX_FULLROWSELECT);
}

UINT TableCtrl::getItemCount()
{
    return ListView_GetItemCount(hWnd);
}

UINT TableCtrl::getColumnCount()
{
    HWND hWndHdr = (HWND)sendMsg(LVM_GETHEADER);
    return SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L);       
}

void TableCtrl::addColumn(LPSTR strColName, int colWidth, int alignment)
{
    LVCOLUMN col;

    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    col.fmt = alignment;
    col.cx = colWidth;
    col.pszText = strColName;
    col.cchTextMax = strlen(strColName);

    ListView_InsertColumn(hWnd, colIdx++, (LPARAM)&col);
}

void TableCtrl::addItem(TableItem *pItem)
{
    pItem->pTableCtrl = this;
    LVITEM item;
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.pszText = LPSTR_TEXTCALLBACK;
    item.lParam = (LPARAM)pItem;
    item.iItem = getItemCount();
    item.iSubItem = 0;

    ListView_InsertItem(hWnd, &item);
}

TableItem *TableCtrl::getItemAt(UINT idx)
{
    LVITEM item;
    item.mask = LVIF_PARAM;
    item.iItem = idx;
    ListView_GetItem(hWnd, &item);
    return (TableItem *)item.lParam;
}

void TableCtrl::onNotify(Event& evt)
{
    LPNMHDR lpHeader = (LPNMHDR)evt.lParam;
    
    if (lpHeader->code == LVN_GETDISPINFO)
    {
        NMLVDISPINFO *plvdi = plvdi = (NMLVDISPINFO *)lpHeader;

        TableItem *pItem = getItemAt(plvdi->item.iItem);

        if (pItem != NULL)
        {
            plvdi->item.pszText = pItem->getColumnData(plvdi->item.iSubItem);
        }
    }
}

int TableCtrl::getSelIndex()
{
    return ListView_GetSelectionMark(hWnd);
}

void TableCtrl::removeItemAt(int index)
{
    TableItem *pItem = getItemAt(index);
    if (pItem != NULL)
    {
        delete pItem;
    }

    ListView_DeleteItem(hWnd, index);
}

void TableCtrl::removeAllItems()
{
    int count = getItemCount();
    for (int i = 0; i < count; i++) {
        TableItem *pItem = getItemAt(i);
        delete pItem;
    }

    ListView_DeleteAllItems(hWnd);
}

void TableCtrl::updateItem(TableItem* pItem)
{
    int count = getItemCount();
    for (int i = 0; i < count; i++) {
        if (getItemAt(i) == pItem) {
            ListView_Update(hWnd, i);
            return;
        }
    }    
}   