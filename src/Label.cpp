#include "Label.h"

Label::Label(LPSTR title, TextAlignment::e align)
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

void Label::onDrawItem(Event &evt)
{
    LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)evt.lParam;
    // showMsg("OK");
    HFONT hFont = (HFONT)sendMsg(WM_GETFONT);
    HDC hDC = lpDrawItem->hDC;
    StrBuffer text;
    getText(text);

    SetBkMode(hDC, TRANSPARENT);
    SelectObject(hDC, hFont);
    SetTextColor(hDC, textColor);
    if (isOpaque)
    {
        HBRUSH hBrush = CreateSolidBrush(backColor);
        FillRect(hDC, &lpDrawItem->rcItem, hBrush);
        DeleteObject(hBrush);
    }

    DrawText(hDC, text.getBuffer(), text.getLength(), &lpDrawItem->rcItem,
             DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | align);
}