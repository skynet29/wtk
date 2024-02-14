#include "Frame.h"

Frame::Frame(LPSTR title)
{
    attr.title = title;    
    attr.style = WS_OVERLAPPEDWINDOW;
    attr.bounds = Bounds(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);    
}

void Frame::setMenu(Menu& menu)
{
    attr.hMenu = menu.hMenu;
}

void Frame::close()
{
    sendMsg(WM_CLOSE);
}

void Frame::handleEvent(Event& evt)
{
    switch(evt.uMsg) {
        case WM_CLOSE:
            if (canClose()) {
                DestroyWindow(hWnd);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_INITMENUPOPUP:
            onInitMenu((HMENU)evt.wParam);
            break;

        default:
            Container::handleEvent(evt);
    }
}

LPSTR Frame::getOpenFileName(LPSTR strFilter, LPSTR strInitDir)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
    static char strFileName[512];
	LPSTR lpTemp = NULL;
	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;

	if (strFilter != NULL)
	{
		lpTemp = strdup(strFilter);

		LPSTR lpTemp2 = lpTemp;
		while (lpTemp2 = strchr(lpTemp2, '|'))
			*lpTemp2++ = 0;

		ofn.lpstrFilter = lpTemp;
	}

	ofn.lpstrFile = strFileName;
	ofn.nMaxFile = sizeof(strFileName);
	//ofn.lpstrDefExt = strDefaultExt;
	ofn.Flags = OFN_HIDEREADONLY;

	ofn.lpstrInitialDir = strInitDir;
    BOOL ret = GetOpenFileName(&ofn);

    if (lpTemp) free(lpTemp);

    return (ret) ? strFileName : NULL;
}

int Frame::showMsg(LPSTR text, LPSTR title, UINT type) 
{
    return MessageBox(hWnd, text, title, type);
}