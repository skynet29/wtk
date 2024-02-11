#include "Container.h"

Container::~Container()
{
    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        delete childs[idx];
    }    
}

void Container::addChild(Window* child)
{
    child->parent = this;
    childs.add(child);
}

void Container::addChild(Window* child, Bounds bounds)
{
    child->setBounds(bounds);
    addChild(child);
}

void Container::handleEvent(TEvent& evt)
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
                Control* pCtrl = (Control*)GetWindowLong(lpDrawItem->hwndItem, GWL_USERDATA);
                if (pCtrl != NULL) {
                    pCtrl->onDrawItem(lpDrawItem);
                }
            }
            break;

        case WM_NOTIFY:
            {
		        LPNMHDR lpHeader = (LPNMHDR) evt.lParam;
                Control* pCtrl = (Control*) GetWindowLong(lpHeader->hwndFrom, GWL_USERDATA);
                if (pCtrl != NULL) {
                    pCtrl->onNotify(lpHeader);
                }
                
            }
            break;

        default:
            CustCtrl::handleEvent(evt);
    }
}

void Container::onCreate()
{
    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        childs[idx]->create(hWnd);
    }
}

void Container::onCommand(UINT id, UINT code)
{
    if (parent != NULL) {
        parent->onCommand(id, code);
    }
}

Size Container::getPackSize()
{
    int maxWidth = 0;
    int maxHeight  = 0;

    for(UINT idx = 0; idx < childs.getCount(); idx++) {
        Bounds bounds = childs[idx]->getBounds();
        int width = bounds.left + bounds.width;
        int height = bounds.top + bounds.height;
        maxWidth = max(maxWidth, width);
        maxHeight = max(maxHeight, height);
    }
    return Size(maxWidth, maxHeight);
}