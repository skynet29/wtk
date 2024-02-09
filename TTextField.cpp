#include "TTextField.h"

TTextField::TTextField(UINT id, DWORD style)
{
    attr.style |= ES_AUTOHSCROLL | style;
    attr.className = "EDIT";
    attr.hMenu = (HMENU)id;
    attr.styleEx = WS_EX_CLIENTEDGE;
}

void TTextField::setMaxChar(UINT maxChar)
{
    sendMsg(EM_LIMITTEXT, maxChar);
}