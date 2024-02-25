#include "Panel.h"
#include "ScrollBar.h"

Panel::Panel(DWORD style)
{
    attr.style |= style;
    setBackColor(Color::WHITE);

    pVertScrollbar = NULL;
    pHorzScrollbar = NULL;
}

Panel::~Panel()
{
    if (pVertScrollbar != NULL)
        delete pVertScrollbar;

    if (pHorzScrollbar != NULL)
        delete pHorzScrollbar;
}

void Panel::setVertScrollbar(UINT pageHeight, UINT inc)
{
    if (pVertScrollbar != NULL)
        delete pVertScrollbar;

    pVertScrollbar = new ScrollBar(hWnd, pageHeight, inc, SB_VERT);
}

void Panel::setHorzScrollbar(UINT pageWidth, UINT inc)
{
    if (pHorzScrollbar != NULL)
        delete pHorzScrollbar;

    pHorzScrollbar = new ScrollBar(hWnd, pageWidth, inc, SB_HORZ);
}

void Panel::redraw(BOOL eraseBkGnd)
{
    InvalidateRect(hWnd, NULL, eraseBkGnd);
}

Graphic *Panel::getGraphic()
{
    return new WndGraphic(hWnd);
}

void Panel::handleEvent(Event &evt)
{
    Point pt(LOWORD(evt.lParam), HIWORD(evt.lParam));

    switch (evt.uMsg)
    {
    case WM_MOUSEMOVE:
        onMouseMove(pt);
        break;

    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        onLButtonDown(pt);
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        onLButtonUp(pt);
        break;

    case WM_RBUTTONDOWN:
        onRButtonDown(pt);
        break;

    case WM_RBUTTONUP:
        onRButtonUp(pt);
        break;

    case WM_KEYUP:
        onKeyUp(evt.wParam);
        break;

    case WM_KEYDOWN:
        onKeyDown(evt.wParam);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        Point pt = getPageOrigin();
        SetWindowOrgEx(ps.hdc, pt.x, pt.y, NULL);
        Graphic gr(ps.hdc);
        onPaint(gr);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:
    {
        if (pVertScrollbar != NULL)
            pVertScrollbar->resize(HIWORD(evt.lParam));

        if (pHorzScrollbar != NULL)
            pHorzScrollbar->resize(LOWORD(evt.lParam));
    }
    break;

    case WM_HSCROLL:
    {
        if (pHorzScrollbar != NULL)
            pHorzScrollbar->scroll(LOWORD(evt.wParam), HIWORD(evt.wParam));
    }
    break;

    case WM_VSCROLL:
    {
        if (pVertScrollbar != NULL)
            pVertScrollbar->scroll(LOWORD(evt.wParam), HIWORD(evt.wParam));
    }
    break;

    default:
        CustCtrl::handleEvent(evt);
    }
}

Point Panel::getPageOrigin()
{
    Point pt;

    if (pHorzScrollbar != NULL)
        pt.x = pHorzScrollbar->getPos();

    if (pVertScrollbar != NULL)
        pt.y = pVertScrollbar->getPos();

    return pt;
}