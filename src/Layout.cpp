#include "Layout.h"
#include "Button.h"

Layout::Layout(Container* pContainer, int left, int top, LayoutType type)
{
    this->left = left;
    this->top = top;
    this->pContainer = pContainer;
    this->type = type;
}

void Layout::add(Window* pCtrl, Size size, int pad)
{
    pContainer->addChild(pCtrl, Bounds(left, top, size.width, size.height));
    if (type == K_HORIZONTAL) {
        left += size.width + pad;
    }
    else {
        top += size.height + pad;
    }
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