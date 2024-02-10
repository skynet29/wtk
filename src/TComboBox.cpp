#include "TComboBox.h"

TComboBox::TComboBox(UINT id)
{
    attr.style |= CBS_DROPDOWNLIST | WS_VSCROLL;
    attr.className = "COMBOBOX";
    attr.hMenu = (HMENU)id;
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void TComboBox::create(HWND hParent)
{
    attr.bounds.height = 200;
    
    TControl::create(hParent);
}

void TComboBox::addItem(LPSTR str)
{
    sendMsg(CB_ADDSTRING, 0, (LPARAM) str);
}

int TComboBox::getSelIndex()
{
    return sendMsg(CB_GETCURSEL);
}

void TComboBox::getItemAt(UINT idx, TStrBuffer& buff)
{
    int size = sendMsg(CB_GETLBTEXTLEN, idx);
    if (size > 0) {
        buff.allocate(size + 1);
        sendMsg(CB_GETLBTEXT, idx, (LPARAM) buff.getBuffer());
    }
}

void TComboBox::removeAllItems()
{
	sendMsg(CB_RESETCONTENT);
}

void TComboBox::removeItemAt(UINT idx)
{
	sendMsg(CB_DELETESTRING, idx);
}

UINT TComboBox::getItemCount()
{
	return sendMsg(CB_GETCOUNT);
}

void TComboBox::insertItemAt(LPSTR strItem, UINT idx)
{
	sendMsg(CB_INSERTSTRING, idx, (LPARAM)strItem);
}

void TComboBox::setSelIndex(int index)
{
	sendMsg(CB_SETCURSEL, index);
}