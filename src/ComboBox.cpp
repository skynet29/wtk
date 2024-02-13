#include "ComboBox.h"
#include "Container.h"

ComboBox::ComboBox(UINT id)
{
    attr.style |= CBS_DROPDOWNLIST | WS_VSCROLL;
    attr.className = "COMBOBOX";
    attr.hMenu = (HMENU)id;
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void ComboBox::create(HWND hParent)
{
    attr.bounds.height = 200;
    
    Control::create(hParent);
}

void ComboBox::addItem(LPSTR str)
{
    sendMsg(CB_ADDSTRING, 0, (LPARAM) str);
}

int ComboBox::getSelIndex()
{
    return sendMsg(CB_GETCURSEL);
}

void ComboBox::getItemAt(UINT idx, StrBuffer& buff)
{
    int size = sendMsg(CB_GETLBTEXTLEN, idx);
    if (size > 0) {
        buff.allocate(size + 1);
        sendMsg(CB_GETLBTEXT, idx, (LPARAM) buff.getBuffer());
    }
}

void ComboBox::removeAllItems()
{
	sendMsg(CB_RESETCONTENT);
}

void ComboBox::removeItemAt(UINT idx)
{
	sendMsg(CB_DELETESTRING, idx);
}

UINT ComboBox::getItemCount()
{
	return sendMsg(CB_GETCOUNT);
}

void ComboBox::insertItemAt(LPSTR strItem, UINT idx)
{
	sendMsg(CB_INSERTSTRING, idx, (LPARAM)strItem);
}

void ComboBox::setSelIndex(int index)
{
	sendMsg(CB_SETCURSEL, index);
}

void ComboBox::getSelItem(StrBuffer& text) 
{
    int idx = getSelIndex();
    if (idx >= 0) {
        getItemAt(idx, text);
    }
}

void ComboBox::onCommand(Event& evt)
{
    if (HIWORD(evt.wParam) == CBN_SELCHANGE)
        parent->onSelChange(LOWORD(evt.wParam));
}