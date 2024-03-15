#include "CustCtrl.h"
#include "WavePlayer.h"

#define MAINCLASSNAME "MDZ_WINDOW"


CustCtrl::CustCtrl()
{
    backColor = Color::WHITE;
    attr.className = MAINCLASSNAME;
    hCursor = LoadCursor(NULL, IDC_ARROW);
}

 void CustCtrl::setCursor(UINT resId)
 {
    hCursor = LoadCursor(NULL, MAKEINTRESOURCE(resId));
 }

BOOL CustCtrl::registerMainWindow()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = MAINCLASSNAME;
    wc.lpfnWndProc = (WNDPROC) wndProc;
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND+1);

    return RegisterClass(&wc);
}

LRESULT CustCtrl::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CustCtrl* pCtrl = (CustCtrl*)GetWindowLong(hWnd, GWL_USERDATA);
    Event evt(hWnd, uMsg, wParam, lParam);

   if (uMsg == WM_CREATE)
   {
      LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;

      if (lpcs->dwExStyle & WS_EX_MDICHILD)
         pCtrl = (CustCtrl*) ((LPMDICREATESTRUCT) lpcs->lpCreateParams)->lParam;
      else
         pCtrl = (CustCtrl*) lpcs->lpCreateParams;

      pCtrl->hWnd = hWnd;
      SetWindowLong(hWnd, GWL_USERDATA, (LONG) pCtrl);
   }    

   if (pCtrl != NULL && evt.uMsg == WM_DESTROY && pCtrl->attr.styleEx & WS_EX_MDICHILD) {
       //MessageBox(NULL, "Destroy", NULL, MB_OK);
       SetWindowLong(hWnd, GWL_USERDATA, 0);
       delete pCtrl;
       pCtrl = NULL;
   }

    if (pCtrl != NULL) {
        pCtrl->handleEvent(evt);
    }
    else {
        evt.processDefault();
    }

    return evt.lResult;
}


void CustCtrl::handleEvent(Event& evt)
{
    switch(evt.uMsg)
    {
        case WM_ERASEBKGND:
            {
                RECT rc;
                GetClientRect(hWnd, &rc);
                HBRUSH hBrush = CreateSolidBrush(backColor);
                FillRect((HDC) evt.wParam, &rc, hBrush);
                DeleteObject(hBrush);                
            }
            break;

        case WM_SETCURSOR:
            {
                if (hCursor != NULL && LOWORD(evt.lParam) == HTCLIENT) {
                    SetCursor(hCursor);
                    evt.lResult = TRUE;
                }
                else {
                    evt.processDefault();
                }
            }
            break;

        case WM_TIMER:
            onTimer(evt.wParam);
            break;

        case WM_CREATE:
            onCreate();
            break;

        case MM_WOM_DONE:
            {
                LPWAVEHDR lpwh = (LPWAVEHDR)evt.lParam;
                WavePlayer *pPlayer = (WavePlayer*)lpwh->dwUser;
                if (pPlayer != NULL) {
                    pPlayer->onWomDone(lpwh);                    
                }
            }
            break;

        default:
            processDefault(evt);
    }
}

void CustCtrl::processDefault(Event& evt)
{
    if (attr.styleEx & WS_EX_MDICHILD) {
        evt.processMdiDefault();
    }
    else {
        evt.processDefault();
    }
}

void CustCtrl::setBackColor(Color backColor)
{
    this->backColor = backColor;
    InvalidateRect(hWnd, NULL, TRUE);
}

void CustCtrl::startTimer(UINT timerId, UINT delayMs)
{
    SetTimer(hWnd, timerId, delayMs, NULL);
}

void CustCtrl::stopTimer(UINT timerId)
{
    KillTimer(hWnd, timerId);
}