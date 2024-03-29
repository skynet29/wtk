#include "Dialog.h"

Dialog::Dialog(LPSTR strTitle)
{
	attr.bounds = Bounds(0, 0, 200, 200);
	attr.title = strTitle;
	attr.styleEx = WS_EX_DLGMODALFRAME;
	attr.style = WS_POPUPWINDOW | WS_CAPTION;
	setBackColor(GetSysColor(COLOR_BTNFACE));
}

Dialog::~Dialog()
{
    DestroyWindow(hWnd);
}

void Dialog::close(int retCode)
{
	sendMsg(WM_CLOSE, retCode);
}

void Dialog::handleEvent(Event& evt)
{
    switch(evt.uMsg) {
        case WM_COMMAND:
            {
                switch(evt.getId()) {
                    case IDOK:
                        close(TRUE);
                        break;
                    case IDCANCEL:
                        close(FALSE);
                        break;
                    default:
                         Container::handleEvent(evt);
                }
            }           
            break;
        case WM_CLOSE:
            PostQuitMessage(evt.wParam);
            break;

        default:
            Container::handleEvent(evt);
    }
}


int Dialog::run()
{
    if (hWnd == NULL) {
        create(GetActiveWindow());
    }

    centerToParent();

	ShowWindow(hWnd, SW_SHOW);
	HWND hParent = GetParent(hWnd);
	EnableWindow(hParent, FALSE);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if(!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	EnableWindow(hParent, TRUE);
	ShowWindow(hWnd, SW_HIDE);
	return msg.wParam;    
}
