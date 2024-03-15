#include "ListBox.h"
#include "Container.h"

ListBox::ListBox(DWORD style)
{
    attr.style |= LBS_NOTIFY | style;
    attr.className = "LISTBOX";
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void ListBox::addItem(LPSTR str)
{
    sendMsg(LB_ADDSTRING, 0, (LPARAM) str);
}

int ListBox::getSelIndex()
{
    return sendMsg(LB_GETCURSEL);
}

void ListBox::getItemAt(UINT idx, StrBuffer& buff)
{
    int size = sendMsg(LB_GETTEXTLEN, idx);
    if (size > 0) {
        buff.allocate(size + 1);
        sendMsg(LB_GETTEXT, idx, (LPARAM) buff.getBuffer());
    }
}

void ListBox::removeAllItems()
{
	sendMsg(LB_RESETCONTENT);
}

void ListBox::removeItemAt(UINT idx)
{
	sendMsg(LB_DELETESTRING, idx);
}

UINT ListBox::getItemCount()
{
	return sendMsg(LB_GETCOUNT);
}

void ListBox::insertItemAt(LPSTR strItem, UINT idx)
{
	sendMsg(LB_INSERTSTRING, idx, (LPARAM)strItem);
}

BOOL ListBox::isItemSelected(UINT idx)
{
	return (sendMsg(LB_GETSEL, idx) > 0);
}

void ListBox::getSelItem(StrBuffer& text) 
{
    int idx = getSelIndex();
    if (idx >= 0) {
        getItemAt(idx, text);
    }
}

void ListBox::onCommand(Event& evt)
{
    switch(evt.getCode()) {
        case LBN_SELCHANGE:
            onSelChange.fire(this);
            break;
        case LBN_DBLCLK:
            onDblClick.fire(this);
            break;
    }
       
}