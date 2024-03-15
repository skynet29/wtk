#include "Container.h"
#include "Control.h"
#include "Event.h"
#include "Icon.h"
#include "Graphic.h"
#include "Menu.h"

Container::~Container()
{
    for (UINT idx = 0; idx < childs.getCount(); idx++)
    {
        delete childs[idx];
    }
}

void Container::addChild(Window *child)
{
    child->setParent(this);
    childs.add(child);
}

void Container::addChild(Window *child, Bounds bounds)
{
    child->setBounds(bounds);
    addChild(child);
}


void Container::handleEvent(Event &evt)
{
    switch (evt.uMsg)
    {
    case WM_COMMAND:
        {
            //debugPrint("WM_COMMAND id=%d\n", evt.getId());
            Control *pCtrl = (Control *)GetWindowLong((HWND)evt.lParam, GWL_USERDATA);
            if (pCtrl != NULL)
                pCtrl->onCommand(evt);
            else {
                MenuItem* pItem = MenuItem::getFromId(evt.getId());
                if (pItem != NULL)
                    pItem->onClick.fire(pItem);
            }  
        }
        break;

    case WM_INITMENUPOPUP:
        {
            MENUINFO info;
            info.cbSize = sizeof(info);
            info.fMask = MIM_MENUDATA;
            GetMenuInfo((HMENU)evt.wParam, &info);
            PopupMenu* pMenu = (PopupMenu*)info.dwMenuData;
            //debugPrint("pMenu=%p\n", pMenu);
            if (pMenu != NULL)								
                pMenu->onInit.fire(pMenu);
        }
        break;    

    case WM_SIZE:
        onSize(LOWORD(evt.lParam), HIWORD(evt.lParam));
        if (attr.styleEx & WS_EX_MDICHILD)
        {
            evt.processMdiDefault();
        }
        break;

    case WM_SETFOCUS:
        onFocus();
        break;

    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)evt.lParam;

            if (lpdis->CtlType == ODT_MENU) {
                MenuItem* pItem = MenuItem::getFromId(lpdis->itemID);
                if (pItem != NULL) {
                    Graphic gr(lpdis->hDC);
                    gr.drawIcon(Point(lpdis->rcItem.left, lpdis->rcItem.top), pItem->pIcon);
                }

            }
            else {
                Control *pCtrl = (Control *)GetWindowLong(lpdis->hwndItem, GWL_USERDATA);
                if (pCtrl != NULL)
                {
                    pCtrl->onDrawItem(evt);
                }
            }
        }
        break;

    case WM_MEASUREITEM:
        {
            LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)evt.lParam;

            if (lpmis->CtlType == ODT_MENU) {
                MenuItem* pItem = MenuItem::getFromId(lpmis->itemID);
                Size sz = pItem->pIcon->getSize();
                lpmis->itemWidth = sz.width;
                lpmis->itemHeight = sz.height;
            }
            else {
                // Control *pCtrl = (Control *)GetWindowLong(lpmis->hwndItem, GWL_USERDATA);
                // if (pCtrl != NULL)
                // {
                //     pCtrl->onMeasureItem(evt);
                // }
            }
        }
        break;    

    case WM_NOTIFY:
        {
            LPNMHDR lpHeader = (LPNMHDR)evt.lParam;
            Control *pCtrl = (Control *)GetWindowLong(lpHeader->hwndFrom, GWL_USERDATA);
            if (pCtrl != NULL)
            {
                pCtrl->onNotify(evt);
            }
        }
        break;

    case WM_HSCROLL:
        {
            Control *pCtrl = (Control *)GetWindowLong((HWND)evt.lParam, GWL_USERDATA);
            if (pCtrl != NULL)
                pCtrl->onHScroll(evt);
        }
        break;

    case WSA_EVENT:
        {
            switch (WSAGETSELECTEVENT(evt.lParam))
            {
            case FD_ACCEPT:
            {
                SOCKET sock = accept(evt.wParam, NULL, NULL);
                WSAAsyncSelect(sock, hWnd, WSA_EVENT, FD_READ | FD_CLOSE);
                onIncomingConnection(evt.wParam, sock);
            }
            break;

            case FD_READ:
                onDataReceived(evt.wParam);
                break;

            case FD_CLOSE:
                onConnectionClosed(evt.wParam);
                break;
            }
        }
        break;

    default:
        CustCtrl::handleEvent(evt);
    }
}

void Container::onCreate()
{
    for (UINT idx = 0; idx < childs.getCount(); idx++)
    {
        childs[idx]->create(hWnd);
    }
}


Size Container::getPackSize()
{
    int maxWidth = 0;
    int maxHeight = 0;

    for (UINT idx = 0; idx < childs.getCount(); idx++)
    {
        Bounds bounds = childs[idx]->getBounds();
        int width = bounds.left + bounds.width;
        int height = bounds.top + bounds.height;
        maxWidth = max(maxWidth, width);
        maxHeight = max(maxHeight, height);
    }
    return Size(maxWidth, maxHeight);
}

void Container::packSize(int xPad, int yPad)
{
    Size sz = getPackSize();

    Rect rc(0, 0, sz.width + xPad, sz.height + yPad);
    AdjustWindowRectEx(&rc, attr.style, attr.hMenu != NULL, attr.styleEx);
    setSize(rc.getWidth(), rc.getHeight());
}