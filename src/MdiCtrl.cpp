#include "MdiCtrl.h"
#include "Menu.h"

MdiCtrl::MdiCtrl()
{
	attr.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	attr.className = "MDICLIENT";
	attr.param = &ccs;

	ccs.hWindowMenu = NULL;
	ccs.idFirstChild = 500;    
}

void MdiCtrl::cascade()
{
	sendMsg(WM_MDICASCADE);
}

void MdiCtrl::tileHorizontally()
{
	sendMsg(WM_MDITILE, MDITILE_HORIZONTAL);
}

void MdiCtrl::tileVertically()
{
   sendMsg(WM_MDITILE, MDITILE_VERTICAL);
}

void MdiCtrl::setWndMenu(PopupMenu &menu)
{
	ccs.hWindowMenu = menu.getHandle();
}

MdiChild* MdiCtrl::getActiveChild()
{
    return (MdiChild*) GetWindowLong((HWND)sendMsg(WM_MDIGETACTIVE), GWL_USERDATA);
}

void MdiCtrl::createChild(MdiChild *pChild)
{
	pChild->create(hWnd);
}

void MdiCtrl::processDefault(Event& evt) 
{
    if (evt.uMsg == WM_COMMAND) {
        evt.processDefault(hWnd);
    }
}

//////////////////////////////////

MdiChild::MdiChild(LPSTR title)
{
	attr.bounds = Bounds(CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);
	attr.styleEx = WS_EX_MDICHILD;
    attr.title = title;
}