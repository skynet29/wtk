#include "TabCtrl.h"

TabCtrl::TabCtrl()
{
    attr.className = WC_TABCONTROL;

    selIndex = 0;
}

TabCtrl::~TabCtrl()
{
    for(UINT i = 0; i < tabs.getCount(); i++) {
        delete tabs[i];
    }
}

void TabCtrl::addTab(LPSTR strCaption, Window* pCtrl)
{
	Rect rc;
	GetClientRect(hWnd, &rc);
    HWND hParent = GetParent(hWnd);

    TC_ITEM item;
    item.mask = TCIF_TEXT;
    item.pszText = strCaption;
    item.cchTextMax = strlen(strCaption);
    int i = TabCtrl_GetItemCount(hWnd);
    TabCtrl_InsertItem(hWnd, i, &item);
    TabCtrl_AdjustRect(hWnd, FALSE, &rc);
    int width = rc.getWidth();
    int height = rc.getHeight();
    
    ClientToScreen(hWnd, (LPPOINT)&rc);
    ScreenToClient(hParent, (LPPOINT)&rc);

    tabs.add(pCtrl);

    pCtrl->setParent(parent);
    pCtrl->setBounds(Bounds(rc.left, rc.top, width, height));
    pCtrl->setVisible(i == 0);
    pCtrl->create(hParent);
}

Window* TabCtrl::getSelTab()
{
	return tabs[selIndex];
}

void TabCtrl::onNotify(Event& evt)
{
    LPNMHDR lpHeader = (LPNMHDR)evt.lParam;    

	if (lpHeader->code == TCN_SELCHANGE)
	{
		getSelTab()->setVisible(FALSE);
		selIndex = TabCtrl_GetCurSel(hWnd);
		getSelTab()->setVisible(TRUE);
	}
}

void TabCtrl::setSize(int width, int height)
{
    Control::setSize(width, height);

    HWND hParent = GetParent(hWnd);

	Rect rc;    
	GetClientRect(hWnd, &rc);

	TabCtrl_AdjustRect(hWnd, FALSE, (LPARAM) &rc);

    width = rc.getWidth();
    height = rc.getHeight();
    
    ClientToScreen(hWnd, (LPPOINT)&rc);
    ScreenToClient(hParent, (LPPOINT)&rc);

	for (UINT i = 0; i < tabs.getCount(); i++)
	{
		tabs[i]->setBounds(Bounds(rc.left, rc.top, width, height));
	}
}