#include "TContainer.h"

TWndAttr::TWndAttr() 
{
    hMenu = NULL;
    title = NULL;
    className = NULL;
    style = 0;
    styleEx = 0;
}   

HWND TWndAttr::create(HWND hParent)
{
    return CreateWindowEx(
        styleEx, 
        className, 
        title, 
        style, 
        bounds.left, 
        bounds.top, 
        bounds.width,
        bounds.height,
        hParent,
        hMenu,
        GetModuleHandle(NULL),
        param
    );
}

void TWndAttr::modifyStyle(DWORD addedStyle, BOOL state)
{
	if (state)
		style |= addedStyle;
	else
		style &= ~addedStyle;
}

///////////////////////////////////////

TWindow::TWindow()
{
    attr.param = this;
    attr.style = WS_CHILD | WS_VISIBLE;
    parent = NULL;
    hWnd = NULL;
}

void TWindow::create(HWND hParent)
{
    //HWND hParent = (parent != NULL) ? parent->getHandle(): NULL;
    hWnd = attr.create(hParent);
    if (hWnd == NULL)  {
        TStrBuffer str;
        str.format("Fail to create window of class DllExport %s", attr.className);
        MessageBox(NULL, str.getBuffer(), NULL, MB_OK);
    }
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
}

void TWindow::setVisible(BOOL isVisible)
{
    attr.modifyStyle(WS_VISIBLE, isVisible);
    ShowWindow(hWnd, (isVisible) ? SW_SHOW : SW_HIDE);
}

LRESULT TWindow::sendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return SendMessage(hWnd, uMsg, wParam, lParam);
}

void TWindow::getText(TStrBuffer& buff)
{
	int length = GetWindowTextLength(hWnd);
	if (length > 0)
	{
        length++;
		buff.allocate(length);
		GetWindowText(hWnd, (LPSTR) buff.getBuffer(), length);
	}
}

void TWindow::setText(LPSTR text)
{
    SetWindowText(hWnd, text);
}

void TWindow::setLocation(int left, int top)
{
	attr.bounds.left = left;
	attr.bounds.top = top;
	SetWindowPos(hWnd, NULL, left, top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void TWindow::setSize(int width, int height)
{
	attr.bounds.width = width;
	attr.bounds.height = height;

	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void TWindow::centerToParent()
{
	TRect rcParent;
	
	if (GetParent(hWnd))
	{
		GetWindowRect(GetParent(hWnd), &rcParent);
	}
	else
	{
		rcParent.bottom = GetSystemMetrics(SM_CYSCREEN);
		rcParent.right = GetSystemMetrics(SM_CXSCREEN);
	}

	TRect rc;
	GetWindowRect(hWnd, &rc);
	int left = rcParent.left + (rcParent.getWidth() - rc.getWidth()) / 2;
	int top = rcParent.top + (rcParent.getHeight() - rc.getHeight()) / 2;
	setLocation(left, top);
}


int TWindow::showMsg(LPSTR text, LPSTR title, UINT type) 
{
    return MessageBox(NULL, text, title, type);
}

TBounds TWindow::getBounds()
{
    return attr.bounds;
}

void TWindow::setBounds(int left, int top, int width, int height)
{
    attr.bounds = TBounds(left, top, width, height);
    MoveWindow(hWnd, left, top, width, height, TRUE);
}

void TWindow::setFocus()
{
	SetFocus(hWnd);
}

TSize TWindow::getRealSize()
{
	TRect rc;
	GetWindowRect(hWnd, &rc);    
    return TSize(rc.getWidth(), rc.getHeight());
}

void TWindow::setBounds(TRect rc)
{
    setBounds(rc.left, rc.top, rc.getWidth(), rc.getHeight());
}
//////////////////////////////////////

void TControl::setEnabled(BOOL isEnabled)
{
    attr.modifyStyle(WS_DISABLED, !isEnabled);
    EnableWindow(hWnd, isEnabled);
}

void TControl::setFont(TFont* font)
{
    sendMsg(WM_SETFONT, (WPARAM) font->getHandle(), TRUE);
}


////////////////////////////////////

TEvent::TEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->hWnd = hWnd;
    this->uMsg = uMsg;
    this->wParam = wParam;
    this->lParam = lParam;
    this->lResult = 0;
}

void TEvent::processDefault(HWND hMdiClient)
{
    lResult = DefFrameProc(hWnd, hMdiClient, uMsg, wParam, lParam);
}

void TEvent::processMdiDefault()
{
    lResult = DefMDIChildProc(hWnd, uMsg, wParam, lParam);
}
///////////////////////////////////////
#define MAINCLASSNAME "MDZ_WINDOW"


TCustCtrl::TCustCtrl()
{
    backColor = GetSysColor(COLOR_BACKGROUND);
    attr.className = MAINCLASSNAME;
}

BOOL TCustCtrl::registerMainWindow()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = MAINCLASSNAME;
    wc.lpfnWndProc = (WNDPROC) wndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND+1);

    return RegisterClass(&wc);
}

LRESULT TCustCtrl::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TCustCtrl* pCtrl = (TCustCtrl*)GetWindowLong(hWnd, GWL_USERDATA);
    TEvent evt(hWnd, uMsg, wParam, lParam);

   if (uMsg == WM_CREATE)
   {
      LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;

      if (lpcs->dwExStyle & WS_EX_MDICHILD)
         pCtrl = (TContainer*) ((LPMDICREATESTRUCT) lpcs->lpCreateParams)->lParam;
      else
         pCtrl = (TContainer*) lpcs->lpCreateParams;

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


void TCustCtrl::handleEvent(TEvent& evt)
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

        case WM_TIMER:
            onTimer(evt.wParam);
            break;

        case WM_CREATE:
            onCreate();
            break;

        default:
            if (attr.styleEx & WS_EX_MDICHILD) {
                evt.processMdiDefault();
            }
            else {
                evt.processDefault();
            }
    }
}

void TCustCtrl::setBackColor(TColor backColor)
{
    this->backColor = backColor;
    InvalidateRect(hWnd, NULL, TRUE);
}

void TCustCtrl::startTimer(UINT timerId, UINT delayMs)
{
    SetTimer(hWnd, timerId, delayMs, NULL);
}

void TCustCtrl::stopTimer(UINT timerId)
{
    KillTimer(hWnd, timerId);
}

