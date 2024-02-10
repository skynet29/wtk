#include "TListBox.h"

TListBox::TListBox(UINT id, DWORD style)
{
    attr.style |= LBS_NOTIFY | style;
    attr.className = "LISTBOX";
    attr.hMenu = (HMENU)id;
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void TListBox::addItem(LPSTR str)
{
    sendMsg(LB_ADDSTRING, 0, (LPARAM) str);
}

int TListBox::getSelIndex()
{
    return sendMsg(LB_GETCURSEL);
}

void TListBox::getItemAt(UINT idx, TStrBuffer& buff)
{
    int size = sendMsg(LB_GETTEXTLEN, idx);
    if (size > 0) {
        buff.allocate(size + 1);
        sendMsg(LB_GETTEXT, idx, (LPARAM) buff.getBuffer());
    }
}

void TListBox::removeAllItems()
{
	sendMsg(LB_RESETCONTENT);
}

void TListBox::removeItemAt(UINT idx)
{
	sendMsg(LB_DELETESTRING, idx);
}

UINT TListBox::getItemCount()
{
	return sendMsg(LB_GETCOUNT);
}

void TListBox::insertItemAt(LPSTR strItem, UINT idx)
{
	sendMsg(LB_INSERTSTRING, idx, (LPARAM)strItem);
}

BOOL TListBox::isItemSelected(UINT idx)
{
	return (sendMsg(LB_GETSEL, idx) > 0);
}