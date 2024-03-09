#include "Event.h"

Event::Event(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->hWnd = hWnd;
    this->uMsg = uMsg;
    this->wParam = wParam;
    this->lParam = lParam;
    this->lResult = 0;
}

void Event::processDefault(HWND hMdiClient)
{
    lResult = DefFrameProc(hWnd, hMdiClient, uMsg, wParam, lParam);
}

void Event::processMdiDefault()
{
    lResult = DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}