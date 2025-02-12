#include "LayoutManager.h"
#include "Container.h"

LayoutManager::LayoutManager(Container* pContainer) 
{
    this->pContainer = pContainer;
    pContainer->pLayoutManager = this;
}


PackManagerData::PackManagerData(Window* pCtrl)
{
    _width = 60;
    _height = 20;
    _expand = FALSE;
    _side = TOP;
    _fill = NONE;
    _anchor = CENTER;
    this->pCtrl = pCtrl;
}


PackManagerData* PackManager::add(Window* pCtrl)
{
    PackManagerData* pData = new PackManagerData(pCtrl);
    pContainer->addChild(pCtrl);
    data.add(pData);
    return pData;
}

PackManager::PackManager(Container* pContainer) : LayoutManager(pContainer)
{
}

void PackManager::computeBounds(UINT width, UINT height)
{
    debugPrint("PackManager::computeBounds %d,%d\n", width, height);
    UINT nbEX = 0;
    UINT nbEY = 0;
    UINT expandWidth = width;
    UINT expandHeight = height;
    UINT i;

    for(i = 0; i < data.getCount(); i++) {
        PackManagerData* pData = data[i];
        BOOL isLast = (i == data.getCount()-1);

        if (pData->_side == PackManagerData::TOP || pData->_side == PackManagerData::BOTTOM) {
            if (pData->_expand) {
                nbEY++;
            }
            else {
                expandHeight -= pData->_height;
            }
            if (isLast) {
                expandWidth -= pData->_width;
            }
        }
        if (pData->_side == PackManagerData::LEFT || pData->_side == PackManagerData::RIGHT) {
            if (pData->_expand) {
                nbEX++;
            }
            else {
                expandWidth -= pData->_width;
            }
            if (isLast) {
               expandHeight -= pData->_height;
            }
        }
    }

    debugPrint("nbEX=%d, nbEY=%d, width=%d, height=%d\n",
        nbEX, nbEY, expandWidth, expandHeight);

    if (nbEX > 0) {
        expandWidth = expandWidth / nbEX;
    }
    if (nbEY > 0) {
        expandHeight = expandHeight / nbEY;
    }

    debugPrint("expandWidth=%d, expandHeight=%d\n",
        expandWidth, expandHeight);

    Rect rc(0, 0, width, height);

    for(i = 0; i < data.getCount(); i++) {
        PackManagerData* pData = data[i];
        UINT ctrlX, ctrlY, ctrlWidth, ctrlHeight;

        switch(pData->_side) {
        case PackManagerData::TOP:
            ctrlWidth = rc.getWidth();
            ctrlHeight = (pData->_expand) ? expandHeight : pData->_height;
            ctrlX = rc.left;
            ctrlY = rc.top;
            rc.top += ctrlHeight;
            break;
        case PackManagerData::BOTTOM:
            ctrlWidth = rc.getWidth();
            ctrlHeight = (pData->_expand) ? expandHeight : pData->_height;
            ctrlX = rc.left;
            ctrlY = rc.bottom - ctrlHeight;
            rc.bottom -= ctrlHeight;
            break;
        case PackManagerData::LEFT:
            ctrlHeight = rc.getHeight();
            ctrlWidth = (pData->_expand) ? expandWidth : pData->_width;
            ctrlX = rc.left;
            ctrlY = rc.top;
            rc.left += ctrlWidth;
            break;
        case PackManagerData::RIGHT:
            ctrlHeight = rc.getHeight();
            ctrlWidth = (pData->_expand) ? expandWidth : pData->_width;
            ctrlX = rc.right - ctrlWidth;
            ctrlY = rc.top;
            rc.right -= ctrlWidth;
            break;
        }  
        debugPrint("step1: x=%d, y=%d, width=%d, height=%d\n", ctrlX, ctrlY, ctrlWidth, ctrlHeight);
        //debugPrint("x=%d, y=%d, width=%d, height=%d\n", x, y, width, height);

        Bounds bds(ctrlX, ctrlY, pData->_width, pData->_height);

        switch(pData->_fill) {
        case PackManagerData::X:
            bds.width = ctrlWidth;
            break;                
        case PackManagerData::BOTH:
            bds.width = ctrlWidth;
            bds.height = ctrlHeight;
            break;                
        case PackManagerData::Y:
            bds.height = ctrlHeight;
            break;
        }
        debugPrint("step2: x=%d, y=%d, width=%d, height=%d\n", bds.left, bds.top, bds.width, bds.height);

        switch(pData->_anchor) {
        case PackManagerData::N:
            bds.left = ctrlX + (ctrlWidth - bds.width) / 2;
            break;
        case PackManagerData::NE:
            bds.left = ctrlX + ctrlWidth - bds.width;
            break;
        case PackManagerData::S:
            bds.top = ctrlY + ctrlHeight - bds.height;
            bds.left = ctrlX + (ctrlWidth - bds.width) / 2;
            break;
        case PackManagerData::SW:
            bds.top = ctrlY + ctrlHeight - bds.height;
            break;
        case PackManagerData::SE:
            bds.top = ctrlY + ctrlHeight - bds.height;
            bds.left = ctrlX + ctrlWidth - bds.width;
            break;                
        case PackManagerData::W:
            bds.top = ctrlY + (ctrlHeight - bds.height) / 2;
            break;
        case PackManagerData::E:
            bds.top = ctrlY + (ctrlHeight - bds.height) / 2;
            bds.left = ctrlX + ctrlWidth - bds.width;
            break;
        case PackManagerData::CENTER:
            bds.top = ctrlY + (ctrlHeight - bds.height) / 2;
            bds.left = ctrlX + (ctrlWidth - bds.width) / 2;
            break;
        }
        debugPrint("step3: x=%d, y=%d, width=%d, height=%d\n\n", bds.left, bds.top, bds.width, bds.height);
        pData->pCtrl->setBounds(bds);
    }
}