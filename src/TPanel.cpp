#include "TPanel.h"

TPanel::TPanel(DWORD style)
{
    attr.style |= style;
    setBackColor(TColor::WHITE);
}

TGraphic* TPanel::getGraphic()
{
    return new TWndGraphic(hWnd);
}

void TPanel::handleEvent(TEvent& evt)
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
                TGraphic gr(ps.hdc);
                onPaint(gr);
                EndPaint(hWnd, &ps);
            }
            break;

        default:
            TCustCtrl::handleEvent(evt);
    }
}