#include "TApplication.h"
#include "TMenu.h"
#include "TFont.h"

TApplication::TApplication() 
{
    TFont::initClass();
}


TApplication::~TApplication() 
{

}



int TApplication::run(TFrame& frame)
{
    if (!TCustCtrl::registerMainWindow()) {
        MessageBox(NULL, "Pb registering window class", NULL, MB_OK | MB_ICONERROR);
        return -1;
    }

    frame.create(NULL);

    HACCEL hAccel = TShortcut::createAccelTable();

    frame.setVisible(TRUE);
    UpdateWindow(frame.hWnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(frame.hWnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyAcceleratorTable(hAccel);

    return msg.wParam;    

}