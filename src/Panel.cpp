#include "Panel.h"

Panel::Panel(DWORD style)
{
    attr.style |= style;
    setBackColor(Color::WHITE);
}

Graphic* Panel::getGraphic()
{
    return new WndGraphic(hWnd);
}

void Panel::handleEvent(TEvent& evt)
{
    switch(evt.uMsg)
    {
        case WM_MOUSEMOVE:
            onMouseMove(LOWORD(evt.lParam), HIWORD(evt.lParam));
            break;

        case WM_LBUTTONDOWN:
            onLButtonDown(LOWORD(evt.lParam), HIWORD(evt.lParam));
            break;

        case WM_LBUTTONUP:
            onLButtonUp(LOWORD(evt.lParam), HIWORD(evt.lParam));
            break;

        case WM_RBUTTONDOWN:
            onRButtonDown(LOWORD(evt.lParam), HIWORD(evt.lParam));
            break;

        case WM_RBUTTONUP:
            onRButtonUp(LOWORD(evt.lParam), HIWORD(evt.lParam));
            break;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                Graphic gr(ps.hdc);
                onPaint(gr);
                EndPaint(hWnd, &ps);
            }
            break;

        default:
            CustCtrl::handleEvent(evt);
    }
}