#include "TFrame.h"

TFrame::TFrame(LPSTR title)
{
    attr.title = title;    
    attr.style = WS_OVERLAPPEDWINDOW;
    attr.bounds = TBounds(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);    
}

void TFrame::setMenu(TMenu& menu)
{
    attr.hMenu = menu.hMenu;
}

void TFrame::close()
{
    sendMsg(WM_CLOSE);
}

void TFrame::handleEvent(TEvent& evt)
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

        default:
            TContainer::handleEvent(evt);
    }
}

LPSTR TFrame::getOpenFileName(LPSTR strFilter, LPSTR strInitDir)
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