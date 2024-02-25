#include "Layout.h"
#include "Button.h"

Layout::Layout(Container* pContainer, int left, int top)
{
    this->left = left;
    this->top = top;
    this->right = 0;
    this->curX = left;
    this->curY = top;
    this->pContainer = pContainer;
    this->maxLineHeight = 0;
}

void Layout::add(Window* pCtrl, Size size, int pad)
{
    curX += pad;
    pContainer->addChild(pCtrl, Bounds(curX, curY, size.width, size.height));
    curX += size.width;
    right = max(right, curX);
    maxLineHeight = max(maxLineHeight, size.height);
}

void Layout::addEnd(Window* pCtrl, int height, int pad)
{
    curX += pad;
    int width = right - left;
    pContainer->addChild(pCtrl, Bounds(curX, curY, width - curX + left, height));
    curX = left;
    maxLineHeight = max(maxLineHeight, height);
    curY += maxLineHeight;
    maxLineHeight = 0;
}

void Layout::addRight(Window* pCtrl, Size size)
{
    int width = right - left;
    curX += width - size.width;

    pContainer->addChild(pCtrl, Bounds(curX, curY, size.width, size.height));
    curX = left;
    maxLineHeight = max(maxLineHeight, size.height);
    curY += maxLineHeight;
    maxLineHeight = 0;
}

void Layout::endl(int pad) 
{
    curX = left;
    curY += maxLineHeight + pad;
    maxLineHeight = 0;
}

void Layout::addLabel(LPSTR strText, UINT width, UINT height, int pad) 
{
    if (width == 0) {
        Font* pFont = Font::getDefaultGuiFont();
        Size sz = pFont->measureString(strText);
        width = sz.width;
    }
    add(new Label(strText), Size(width, height), pad);
}

void Layout::addBorder(LPSTR strText, int pad) 
{
    Rect rc(left, top, right, curY + maxLineHeight);
    rc.inflate(pad, pad);
    rc.top -= pad / 2;
    pContainer->addChild(new Border(strText), Bounds(rc.left, rc.top, rc.getWidth(), rc.getHeight()));
}