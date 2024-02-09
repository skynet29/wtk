#include "TMdiCtrl.h"
#include "TMenu.h"

TMdiCtrl::TMdiCtrl()
{
	attr.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	attr.className = "MDICLIENT";
	attr.param = &ccs;

	ccs.hWindowMenu = NULL;
	ccs.idFirstChild = 500;    
}

void TMdiCtrl::cascade()
{
	sendMsg(WM_MDICASCADE);
}

void TMdiCtrl::tileHorizontally()
{
	sendMsg(WM_MDITILE, MDITILE_HORIZONTAL);
}

void TMdiCtrl::tileVertically()
{
   sendMsg(WM_MDITILE, MDITILE_VERTICAL);
}

void TMdiCtrl::setWndMenu(TPopupMenu &menu)
{
	ccs.hWindowMenu = menu.getHandle();
}

TMdiChild* TMdiCtrl::getActiveChild()
{
    return (TMdiChild*) GetWindowLong((HWND)sendMsg(WM_MDIGETACTIVE), GWL_USERDATA);
}

void TMdiCtrl::createChild(TMdiChild *pChild)
{
	pChild->create(hWnd);
}

void TMdiCtrl::processDefault(TEvent& evt) 
{
    if (evt.uMsg == WM_COMMAND) {
        evt.processDefault(hWnd);
    }
}

//////////////////////////////////

TMdiChild::TMdiChild(LPSTR title)
{
	attr.bounds = TBounds(CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);
	attr.styleEx = WS_EX_MDICHILD;
    attr.title = title;
}