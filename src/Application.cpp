#include "Application.h"
#include "Menu.h"
#include "Font.h"
#include "TcpSocket.h"

#include <stdio.h>

static Frame* pMainFrame = NULL;

Application::Application() 
{
    Font::initClass();

    Socket::initClass();

    INITCOMMONCONTROLSEX iccex; 

    /* INITIALIZE COMMON CONTROLS */
    iccex.dwICC = ICC_BAR_CLASSES | ICC_COOL_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES ;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&iccex);
        
}

void Application::getExeName(StrBuffer& str)
{
	char buffer[1024];
	GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof(buffer));
    str.set(buffer);
}



Application::~Application() 
{

}

Size Application::getScreenSize()
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy =  GetSystemMetrics(SM_CYSCREEN);
	return Size(cx, cy);
}

int Application::run(Frame& frame)
{
    if (!CustCtrl::registerMainWindow()) {
        MessageBox(NULL, "Pb registering window class", NULL, MB_OK | MB_ICONERROR);
        return -1;
    }

    pMainFrame = &frame;

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

Frame* Application::getMainFrame()
{
    return pMainFrame;
}