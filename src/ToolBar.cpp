#include "ToolBar.h"
#include "Bitmap.h"
#include "Container.h"

const UINT ToolBar::K_BUTTON       = TBSTYLE_BUTTON;
const UINT ToolBar::K_CHECK		= TBSTYLE_CHECK;
const UINT ToolBar::K_CHECKGROUP	= TBSTYLE_CHECKGROUP;

ToolBar::ToolBar()
{
    attr.className = TOOLBARCLASSNAME;
    attr.style |= TBSTYLE_TOOLTIPS;
}

void ToolBar::create(HWND hParent)
{
    Control::create(hParent);

    sendMsg(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON));

    HWND hTooltips = (HWND) sendMsg(TB_GETTOOLTIPS);
	SetWindowLong(hTooltips, GWL_USERDATA, (LONG) this);

    sendMsg(TB_LOADIMAGES,  (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
}

void ToolBar::addButton(Bitmap* pBitmap, int idCommand, LPSTR strToolTips, BYTE style)
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

void ToolBar::addStdButton(int idx, int idCommand, LPSTR strToolTips, BYTE style)
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

void ToolBar::addSeparator(int size)
{
    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.fsStyle = TBSTYLE_SEP;
	tbb.iBitmap = size;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);   
}

void ToolBar::seButtonEnabled(int idCommand, BOOL isEnabled)
{
    sendMsg(TB_ENABLEBUTTON, idCommand, MAKELONG(isEnabled, 0));
}

void ToolBar::seButtonChecked(int idCommand, BOOL isChecked)
{
    sendMsg(TB_CHECKBUTTON, idCommand, MAKELONG(isChecked, 0));
}

BOOL ToolBar::isButtonChecked(int idCommand)
{
	int index = sendMsg(TB_COMMANDTOINDEX, idCommand);
	TBBUTTON tbb;
	sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_CHECKED) != 0);
}

BOOL ToolBar::isButtonEnabled(int idCommand)
{
	int index = sendMsg(TB_COMMANDTOINDEX, idCommand);
	TBBUTTON tbb;
	sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_ENABLED) != 0);
}

void ToolBar::onCommand(Event& evt)
{
    parent->onCommand(LOWORD(evt.wParam));
}

void ToolBar::onNotify(Event& evt)
{
    LPNMHDR lpHeader = (LPNMHDR)evt.lParam;
//    StrBuffer str;
//    str.format("ToolBar::onNotify id=%d, code=%d", lpHeader->idFrom, lpHeader->code);
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

Size ToolBar::resize()
{
    sendMsg(WM_SIZE);
    return getRealSize();
}