#include "TToolBar.h"
#include "TBitmap.h"

#include <commctrl.h>

const UINT TToolBar::K_BUTTON       = TBSTYLE_BUTTON;
const UINT TToolBar::K_CHECK		= TBSTYLE_CHECK;
const UINT TToolBar::K_CHECKGROUP	= TBSTYLE_CHECKGROUP;

TToolBar::TToolBar()
{
    attr.className = TOOLBARCLASSNAME;
    attr.style |= TBSTYLE_TOOLTIPS;
}

void TToolBar::create(HWND hParent)
{
    TControl::create(hParent);

    sendMsg(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON));

    HWND hTooltips = (HWND) sendMsg(TB_GETTOOLTIPS);
	SetWindowLong(hTooltips, GWL_USERDATA, (LONG) this);

    sendMsg(TB_LOADIMAGES,  (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
}

void TToolBar::addButton(TBitmap* pBitmap, int idCommand, LPSTR strToolTips, BYTE style)
{
	TBADDBITMAP bmp;

	bmp.hInst = NULL ;
	bmp.nID = (UINT) pBitmap->getHandle();

    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.idCommand = idCommand;
	tbb.iBitmap = sendMsg(TB_ADDBITMAP, 1, (LPARAM) &bmp);    
	tbb.fsState = TBSTATE_ENABLED;
    tbb.fsStyle = style; 
    tbb.dwData = (strToolTips != NULL) ? (DWORD)strdup(strToolTips) : 0;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);
}

void TToolBar::addStdButton(int idx, int idCommand, LPSTR strToolTips, BYTE style)
{
    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.idCommand = idCommand;
    tbb.iBitmap = MAKELONG(idx, 0);
	tbb.fsState = TBSTATE_ENABLED;
    tbb.fsStyle = style; 
    tbb.dwData = (strToolTips != NULL) ? (DWORD)strdup(strToolTips) : 0;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);   
}

void TToolBar::addSeparator(int size)
{
    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.fsStyle = TBSTYLE_SEP;
	tbb.iBitmap = size;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);   
}

void TToolBar::setButtonEnabled(int idCommand, BOOL isEnabled)
{
    sendMsg(TB_ENABLEBUTTON, idCommand, MAKELONG(isEnabled, 0));
}

void TToolBar::setButtonChecked(int idCommand, BOOL isChecked)
{
    sendMsg(TB_CHECKBUTTON, idCommand, MAKELONG(isChecked, 0));
}

BOOL TToolBar::isButtonChecked(int idCommand)
{
	int index = sendMsg(TB_COMMANDTOINDEX, idCommand);
	TBBUTTON tbb;
	sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_CHECKED) != 0);
}

BOOL TToolBar::isButtonEnabled(int idCommand)
{
	int index = sendMsg(TB_COMMANDTOINDEX, idCommand);
	TBBUTTON tbb;
	sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_ENABLED) != 0);
}

void TToolBar::onNotify(LPNMHDR lpHeader)
{
//    TStrBuffer str;
//    str.format("TToolBar::onNotify id=%d, code=%d", lpHeader->idFrom, lpHeader->code);
//    MessageBox(NULL, str.getBuffer(), NULL, MB_OK);
    if (lpHeader->code == TTN_GETDISPINFO) {
        int index = sendMsg(TB_COMMANDTOINDEX, lpHeader->idFrom);
        TBBUTTON tbb;
        sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);   

        if (tbb.dwData != 0) {
    		LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO) lpHeader;
			strcpy(lpnmtdi->szText, (LPSTR) tbb.dwData);	     
        }
    }
}

TSize TToolBar::resize()
{
    sendMsg(WM_SIZE);
    return getRealSize();
}