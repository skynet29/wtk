#include "Window.h"
#include "WavePlayer.h"

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

void Window::clientToScreen(Point& pt)
{
    ClientToScreen(hWnd, &pt);
}

void Window::screenToClient(Point& pt)
{
    ScreenToClient(hWnd, &pt);
}



