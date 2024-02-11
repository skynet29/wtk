#include "TextField.h"

TextField::TextField(UINT id, DWORD style)
{
    attr.style |= ES_AUTOHSCROLL | style;
    attr.className = "EDIT";
    attr.hMenu = (HMENU)id;
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void TextField::setMaxChar(UINT maxChar)
{
    sendMsg(EM_LIMITTEXT, maxChar);
}