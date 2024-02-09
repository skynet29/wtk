#include "TDialog.h"

TDialog::TDialog()
{
	attr.bounds = TBounds(0, 0, 200, 200);
	attr.title = "Dialog";
	attr.styleEx = WS_EX_DLGMODALFRAME;
	attr.style = WS_POPUPWINDOW | WS_CAPTION;
	setBackColor(GetSysColor(COLOR_BTNFACE));
}

TDialog::~TDialog()
{
    DestroyWindow(hWnd);
}

void TDialog::close(int retCode)
{
	sendMsg(WM_CLOSE, retCode);
}

void TDialog::handleEvent(TEvent& evt)
{
    switch(evt.uMsg) {
        case WM_CLOSE:
            PostQuitMessage(evt.wParam);
            break;

        default:
            TContainer::handleEvent(evt);
    }
}


int TDialog::run()
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

void TDialog::onCommand(UINT id, UINT code)
{
    if (id == IDCANCEL) {
        close(FALSE);
    }
}