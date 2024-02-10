#include "TContainer.h"

TContainer::~TContainer()
{
    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        delete childs[idx];
    }    
}

void TContainer::addChild(TWindow* child)
{
    child->parent = this;
    childs.add(child);
}

void TContainer::addChild(TWindow* child, TBounds bounds)
{
    child->setBounds(bounds);
    addChild(child);
}

void TContainer::handleEvent(TEvent& evt)
{
    switch(evt.uMsg)
    {
        case WM_COMMAND:
            onCommand(LOWORD(evt.wParam), HIWORD(evt.wParam));
            break;

        case WM_SIZE:
            onSize(LOWORD(evt.lParam), HIWORD(evt.lParam));
            if (attr.styleEx & WS_EX_MDICHILD) {
                evt.processMdiDefault();
            }
            break;

        case WM_SETFOCUS:
            onFocus();
            break;

        case WM_DRAWITEM:
            {
                LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT) evt.lParam;
                TControl* pCtrl = (TControl*)GetWindowLong(lpDrawItem->hwndItem, GWL_USERDATA);
                if (pCtrl != NULL) {
                    pCtrl->onDrawItem(lpDrawItem);
                }
            }
            break;

        case WM_NOTIFY:
            {
		        LPNMHDR lpHeader = (LPNMHDR) evt.lParam;
                TControl* pCtrl = (TControl*) GetWindowLong(lpHeader->hwndFrom, GWL_USERDATA);
                if (pCtrl != NULL) {
                    pCtrl->onNotify(lpHeader);
                }
                
            }
            break;

        default:
            TCustCtrl::handleEvent(evt);
    }
}

void TContainer::onCreate()
{
    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        childs[idx]->create(hWnd);
    }
}

void TContainer::onCommand(UINT id, UINT code)
{
    if (parent != NULL) {
        parent->onCommand(id, code);
    }
}

TSize TContainer::getPackSize()
{
    int maxWidth = 0;
    int maxHeight  = 0;

    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        TBounds bounds = childs[idx]->getBounds();
        int width = bounds.left + bounds.width;
        int height = bounds.top + bounds.height;
        maxWidth = max(maxWidth, width);
        maxHeight = max(maxHeight, height);
    }
    return TSize(maxWidth, maxHeight);
}