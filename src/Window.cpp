#include "Container.h"
#include "WavePlayer.h"

#define MAINCLASSNAME "MDZ_WINDOW"


WndAttr::WndAttr() 
{
    hMenu = NULL;
    title = NULL;
    className = NULL;
    style = 0;
    styleEx = 0;
}   

HWND WndAttr::create(HWND hParent)
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

void WndAttr::modifyStyle(DWORD addedStyle, BOOL state)
{
	if (state)
		style |= addedStyle;
	else
		style &= ~addedStyle;
}

///////////////////////////////////////

Window::Window()
{
    attr.param = this;
    attr.style = WS_CHILD | WS_VISIBLE;
    parent = NULL;
    hWnd = NULL;
}

void Window::create(HWND hParent)
{
    //HWND hParent = (parent != NULL) ? parent->getHandle(): NULL;
    hWnd = attr.create(hParent);
    if (hWnd == NULL)  {
        StrBuffer str;
        str.format("Fail to create window of class DllExport %s", attr.className);
        MessageBox(NULL, str.getBuffer(), NULL, MB_OK);
    }
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
}

void Window::setVisible(BOOL isVisible)
{
    attr.modifyStyle(WS_VISIBLE, isVisible);
    ShowWindow(hWnd, (isVisible) ? SW_SHOW : SW_HIDE);
}

LRESULT Window::sendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return SendMessage(hWnd, uMsg, wParam, lParam);
}

void Window::getText(StrBuffer& buff)
{
	int length = GetWindowTextLength(hWnd);
	if (length > 0)
	{
        length++;
		buff.allocate(length);
		GetWindowText(hWnd, (LPSTR) buff.getBuffer(), length);
	}
}

void Window::setText(LPSTR text)
{
    attr.title = text;
    SetWindowText(hWnd, text);
}

void Window::setLocation(int left, int top)
{
	attr.bounds.left = left;
	attr.bounds.top = top;
	SetWindowPos(hWnd, NULL, left, top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void Window::setSize(int width, int height)
{
	attr.bounds.width = width;
	attr.bounds.height = height;

	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void Window::centerToParent()
{
	Rect rcParent;
	
	if (GetParent(hWnd))
	{
		GetWindowRect(GetParent(hWnd), &rcParent);
	}
	else
	{
		rcParent.bottom = GetSystemMetrics(SM_CYSCREEN);
		rcParent.right = GetSystemMetrics(SM_CXSCREEN);
	}

	Rect rc;
	GetWindowRect(hWnd, &rc);
	int left = rcParent.left + (rcParent.getWidth() - rc.getWidth()) / 2;
	int top = rcParent.top + (rcParent.getHeight() - rc.getHeight()) / 2;
	setLocation(left, top);
}

Bounds Window::getBounds()
{
    return attr.bounds;
}

void Window::setBounds(Bounds bounds)
{
    attr.bounds = bounds;
    MoveWindow(hWnd, bounds.left, bounds.top, bounds.width, bounds.height, TRUE);
}

void Window::setFocus()
{
	SetFocus(hWnd);
}

Size Window::getRealSize()
{
	Rect rc;
	GetWindowRect(hWnd, &rc);    
    return Size(rc.getWidth(), rc.getHeight());
}

void Window::setBounds(Rect rc)
{
    setBounds(Bounds(rc.left, rc.top, rc.getWidth(), rc.getHeight()));
}

void Window::setParent(Container* parent)
{
    this->parent = parent;
}

Size Window::getClientSize()
{
    Rect rc;
    GetClientRect(hWnd, &rc);
    return Size(rc.getWidth(), rc.getHeight());
}

BOOL Window::isControl()
{
    return !StrBuffer(attr.className).equals(MAINCLASSNAME);
}

//////////////////////////////////////

UINT Control::getId()
{
    return (UINT)attr.hMenu;
}

void Control::setEnabled(BOOL isEnabled)
{
    attr.modifyStyle(WS_DISABLED, !isEnabled);
    EnableWindow(hWnd, isEnabled);
}

void Control::setFont(Font* font)
{
    sendMsg(WM_SETFONT, (WPARAM) font->getHandle(), TRUE);
}

void Control::create(HWND hParent) {
    Window::create(hParent);
    setFont(Font::getDefaultGuiFont());
}


////////////////////////////////////

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
///////////////////////////////////////


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
         pCtrl = (Container*) ((LPMDICREATESTRUCT) lpcs->lpCreateParams)->lParam;
      else
         pCtrl = (Container*) lpcs->lpCreateParams;

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
            if (attr.styleEx & WS_EX_MDICHILD) {
                evt.processMdiDefault();
            }
            else {
                evt.processDefault();
            }
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
