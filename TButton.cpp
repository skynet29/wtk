#include "TButton.h"

const UINT TLabel::K_LEFT	= DT_LEFT;
const UINT TLabel::K_RIGHT	= DT_RIGHT;
const UINT TLabel::K_CENTER	= DT_CENTER;

TLabel::TLabel(LPSTR title, TColor textColor, UINT align)
{
    attr.className = "STATIC";
    attr.title = title;
    attr.style |= SS_OWNERDRAW;
    this->align = align;
    this->textColor = textColor;
}

void TLabel::onDrawItem(LPDRAWITEMSTRUCT lpDrawItem)
{
    //showMsg("OK");
    HFONT hFont = (HFONT)sendMsg(WM_GETFONT);
    HDC hDC = lpDrawItem->hDC;

    SetBkMode(hDC, TRANSPARENT);
    SelectObject(hDC, hFont);
    SetTextColor(hDC, textColor);
    //FrameRect(hDC, &lpDrawItem->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
    DrawText(hDC, attr.title, strlen(attr.title), &lpDrawItem->rcItem,
         DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | align);
}

TButton::TButton(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_PUSHBUTTON;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;
}

TCheckBox::TCheckBox(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_AUTOCHECKBOX;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;
}

void TCheckBox::setChecked(BOOL isChecked)
{
	sendMsg(BM_SETCHECK, isChecked);
}

BOOL TCheckBox::isChecked()
{
	return (sendMsg(BM_GETCHECK) == BST_CHECKED);;
}

TRadioButton::TRadioButton(LPSTR title, UINT id) : TCheckBox(title, id)
{
    attr.style = WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON;

}

TBorder::TBorder(LPSTR title)
{
    attr.title = title;
    attr.style |= BS_GROUPBOX;
    attr.className = "BUTTON";
}