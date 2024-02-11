#include "Application.h"
#include "Menu.h"
#include "Font.h"

Application::Application() 
{
    Font::initClass();

    INITCOMMONCONTROLSEX iccex; 

    /* INITIALIZE COMMON CONTROLS */
    iccex.dwICC = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES ;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&iccex);
        
}


Application::~Application() 
{

}



int Application::run(Frame& frame)
{
    if (!CustCtrl::registerMainWindow()) {
        MessageBox(NULL, "Pb registering window class", NULL, MB_OK | MB_ICONERROR);
        return -1;
    }

    frame.create(NULL);

    HACCEL hAccel = Shortcut::createAccelTable();

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