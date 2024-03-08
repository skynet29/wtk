
#include <windows.h>

#include "Application.h"
#include "Frame.h"

#include "HorzDividerCtrl.h"
#include "ListBox.h"
#include "TextArea.h"


class MyFrame : public Frame {
public:

    MyFrame() : Frame("Test 9") {
        addChild(pDivider = new HorzDividerCtrl(list1 = new ListBox(0), new TextArea(), 100));
    }
protected:
    HorzDividerCtrl* pDivider;
    ListBox* list1;

    void onSelChange(UINT id) {
        debugPrint("onSelChange id=%d\n", id);
    }

    void onCreate() {
        Frame::onCreate();

        list1->addItem("Marc");
        list1->addItem("Brigitte");
        list1->addItem("Quentin");
    }

    void onSize(UINT width, UINT height) {
        pDivider->setSize(width, height);
    }

};


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Application app;
    MyFrame frame;

    return app.run(frame);
}
