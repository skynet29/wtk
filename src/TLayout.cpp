#include "TLayout.h"

TLayout::TLayout(TContainer* pContainer, int left, int top, TLayoutType type)
{
    this->left = left;
    this->top = top;
    this->pContainer = pContainer;
    this->type = type;
}

void TLayout::add(TWindow* pCtrl, TSize size, int pad)
{
    pContainer->addChild(pCtrl, TBounds(left, top, size.width, size.height));
    if (type == K_HORIZONTAL) {
        left += size.width + pad;
    }
    else {
        top += size.height + pad;
    }
}