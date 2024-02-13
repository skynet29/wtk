#include "Button.h"
#include "Container.h"

const UINT Label::K_LEFT	= DT_LEFT;
const UINT Label::K_RIGHT	= DT_RIGHT;
const UINT Label::K_CENTER	= DT_CENTER;

Label::Label(LPSTR title, Color texColor, UINT align)
{
    attr.className = "STATIC";
    attr.title = title;
    attr.style |= SS_OWNERDRAW;
    this->align = align;
    this->texColor = texColor;
}

void Label::onDrawItem(Event& evt)
{
    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)evt.lParam;
    //showMsg("OK");
    HFONT hFont = (HFONT)sendMsg(WM_GETFONT);
    HDC hDC = lpDrawItem->hDC;

    SetBkMode(hDC, TRANSPARENT);
    SelectObject(hDC, hFont);
    SetTextColor(hDC, texColor);
    //FrameRect(hDC, &lpDrawItem->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
    DrawText(hDC, attr.title, strlen(attr.title), &lpDrawItem->rcItem,
         DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | align);
}

Button::Button(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_PUSHBUTTON;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;    
}

void Button::onCommand(Event& evt)
{
    if (HIWORD(evt.wParam) == BN_CLICKED)
        parent->onCommand(LOWORD(evt.wParam));
}

CheckBox::CheckBox(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_AUTOCHECKBOX;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;
}

void CheckBox::setChecked(BOOL isChecked)
{
	sendMsg(BM_SETCHECK, isChecked);
}

BOOL CheckBox::isChecked()
{
	return (sendMsg(BM_GETCHECK) == BST_CHECKED);;
}

RadioButton::RadioButton(LPSTR title, UINT id) : CheckBox(title, id)
{
    attr.style = WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON;

}

Border::Border(LPSTR title)
{
    attr.title = title;
    attr.style |= BS_GROUPBOX;
    attr.className = "BUTTON";
}