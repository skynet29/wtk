#include "ToolBar.h"
#include "Bitmap.h"
#include "Container.h"
#include "Icon.h"

const UINT ToolBar::K_BUTTON       = TBSTYLE_BUTTON;
const UINT ToolBar::K_CHECK		= TBSTYLE_CHECK;
const UINT ToolBar::K_CHECKGROUP	= TBSTYLE_CHECKGROUP;


ToolButton::ToolButton(ToolBar* parent)
{
    static UINT g_Id = 600;
    this->id = g_Id++;
    this->parent = parent;
}

void ToolButton::setToolTips(LPSTR strToolTips)
{
    toolTips.set(strToolTips);
}

void ToolButton::setEnabled(BOOL isEnabled)
{
    parent->sendMsg(TB_ENABLEBUTTON, id, MAKELONG(isEnabled, 0));
}

void ToolButton::setChecked(BOOL isChecked)
{
    parent->sendMsg(TB_CHECKBUTTON, id, MAKELONG(isChecked, 0));
}

BOOL ToolButton::isChecked()
{
	int index = parent->sendMsg(TB_COMMANDTOINDEX, id);
	TBBUTTON tbb;
	parent->sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_CHECKED) != 0);
}

BOOL ToolButton::isEnabled()
{
	int index = parent->sendMsg(TB_COMMANDTOINDEX, id);
	TBBUTTON tbb;
	parent->sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);

    return ((tbb.fsState & TBSTATE_ENABLED) != 0);
}

///////////////////////

ToolBar::ToolBar()
{
    attr.className = TOOLBARCLASSNAME;
    attr.style |= TBSTYLE_TOOLTIPS;
}

ToolBar::~ToolBar()
{
    delete pImageList;
}

void ToolBar::create(HWND hParent)
{
    Control::create(hParent);

    sendMsg(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON));

    HWND hTooltips = (HWND) sendMsg(TB_GETTOOLTIPS);
	SetWindowLong(hTooltips, GWL_USERDATA, (LONG) this);

    pImageList = new ImageList(16, 16, ILC_COLOR24 | ILC_MASK);

    sendMsg(TB_SETIMAGELIST, 0, (LPARAM)pImageList->getHandle());

    //sendMsg(TB_LOADIMAGES,  (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
}

ToolButton* ToolBar::addButton(Icon* pIcon, BYTE style)
{
    ToolButton *pButton = new ToolButton(this);
    debugPrint("addButton pIcon=%p\n", pIcon);

    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.idCommand = pButton->getId();
	tbb.iBitmap = pImageList->addIcon(pIcon);    
	tbb.fsState = TBSTATE_ENABLED;
    tbb.fsStyle = style; 
    tbb.dwData = (DWORD)pButton;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);
    return pButton;
}


void ToolBar::addSeparator(int size)
{
    TBBUTTON tbb;
    ZeroMemory(&tbb, sizeof(tbb));
	tbb.fsStyle = TBSTYLE_SEP;
	tbb.iBitmap = size;

    sendMsg(TB_ADDBUTTONS, 1, (LPARAM) &tbb);   
}



void ToolBar::onCommand(Event& evt)
{
    //debugPrint("ToolBar::onCommand id=%d\n", evt.getId());
     ToolButton* pButton = getButton(evt.getId());
     if (pButton != NULL) 
        pButton->onClick.fire(pButton);
}

ToolButton* ToolBar::getButton(UINT id)
{
    int index = sendMsg(TB_COMMANDTOINDEX, id);
    TBBUTTON tbb;
    sendMsg(TB_GETBUTTON, index, (LPARAM) &tbb);   
    return (ToolButton*)tbb.dwData;
}

void ToolBar::onNotify(Event& evt)
{
    LPNMHDR lpHeader = (LPNMHDR)evt.lParam;
    debugPrint("ToolBar::onNotify id=%d, code=%d\n", lpHeader->idFrom, lpHeader->code);
    if (lpHeader->code == TTN_GETDISPINFO) {
        
        ToolButton* pButton = getButton(lpHeader->idFrom);
        if (pButton != 0 && pButton->toolTips.getBuffer() != NULL) {
    		LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO) lpHeader;
			strcpy(lpnmtdi->szText, (LPSTR) pButton->toolTips.getBuffer());	     
        }
    }
}

Size ToolBar::resize()
{
    sendMsg(WM_SIZE);
    return getRealSize();
}