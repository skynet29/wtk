#include "Container.h"

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

Control *Container::getControlById(UINT id)
{
    for (UINT idx = 0; idx < childs.getCount(); idx++)
    {
        if (childs[idx]->isControl())
        {
            Control *pCtrl = (Control *)childs[idx];
            if (pCtrl->getId() == id)
                return pCtrl;
        }
    }
    return NULL;
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
        Control *pCtrl = (Control *)GetWindowLong((HWND)evt.lParam, GWL_USERDATA);
        if (pCtrl != NULL)
            pCtrl->onCommand(evt);
        else
            onCommand(LOWORD(evt.wParam));
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
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)evt.lParam;
        Control *pCtrl = (Control *)GetWindowLong(lpDrawItem->hwndItem, GWL_USERDATA);
        if (pCtrl != NULL)
        {
            pCtrl->onDrawItem(evt);
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
            onIncomingConnection(sock);
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

void Container::onCommand(UINT id)
{
    if (parent != NULL)
        parent->onCommand(id);
}

void Container::onSelChange(UINT id)
{
    if (parent != NULL)
        parent->onSelChange(id);
}

void Container::onRightClick(UINT id, Point pt)
{
    if (parent != NULL)
        parent->onRightClick(id, pt);
}

void Container::onDblClick(UINT id)
{
    if (parent != NULL)
        parent->onDblClick(id);
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