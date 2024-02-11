#include "Layout.h"

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