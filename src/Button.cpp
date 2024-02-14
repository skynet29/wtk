#include "Button.h"
#include "Container.h"
#include "Bitmap.h"

const UINT Label::K_LEFT	= DT_LEFT;
const UINT Label::K_RIGHT	= DT_RIGHT;
const UINT Label::K_CENTER	= DT_CENTER;

Label::Label(LPSTR title, UINT align)
{
    attr.className = "STATIC";
    attr.title = title;
    attr.style |= SS_OWNERDRAW;
    this->align = align;
    this->textColor = Color::BLACK;
    this->isOpaque = FALSE;
    this->backColor = Color::WHITE;
}

void Label::setBackColor(Color backColor)
{
    this->backColor = backColor;
    this->isOpaque = TRUE;
    InvalidateRect(hWnd, NULL, TRUE);
}

void Label::setTextColor(Color textColor)
{
    this->textColor = textColor;
    InvalidateRect(hWnd, NULL, TRUE);
}

void Label::onDrawItem(Event& evt)
{
    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)evt.lParam;
    //showMsg("OK");
    HFONT hFont = (HFONT)sendMsg(WM_GETFONT);
    HDC hDC = lpDrawItem->hDC;
    StrBuffer text;
    getText(text);

    SetBkMode(hDC, TRANSPARENT);
    SelectObject(hDC, hFont);
    SetTextColor(hDC, textColor);
    if (isOpaque) {
        HBRUSH hBrush = CreateSolidBrush(backColor);
        FillRect(hDC, &lpDrawItem->rcItem, hBrush);
        DeleteObject(hBrush);
    }
   
    DrawText(hDC, text.getBuffer(), text.getLength(), &lpDrawItem->rcItem,
         DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | align);
}

Button::Button(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_PUSHBUTTON;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;    
}

void Button::setBitmap(Bitmap* pBitmap)
{
	sendMsg(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) pBitmap->getHandle());
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