#include "TextField.h"

TextField::TextField(DWORD style)
{
    attr.style |= ES_AUTOHSCROLL | style;
    attr.className = "EDIT";
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void TextField::setMaxChar(UINT maxChar)
{
    sendMsg(EM_LIMITTEXT, maxChar);
}