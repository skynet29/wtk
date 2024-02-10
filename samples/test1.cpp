
#include <windows.h>

#include "TApplication.h"
#include "TFrame.h"
#include "TStrBuffer.h"
#include "TToolBar.h"
#include "TMdiCtrl.h"
#include "TTextArea.h"

enum {
    ID_FILEOPEN = 200,
    ID_FILENEW,
    ID_FILESAVE,
    ID_CASCADE,
    ID_TILEVERTICAL,
    ID_TILEHORIZONTAL,
    ID_EXIT
};

class MyChild : public TMdiChild {
private:
    TTextArea* pTextArea;
public: 
    MyChild(LPSTR title) : TMdiChild(title) {
        addChild(pTextArea = new TTextArea());
    }

protected:
    void onSize(UINT width, UINT height) {
        pTextArea->setSize(width, height);
    }

    void onFocus() {
        pTextArea->setFocus();
    }
};


class MyFrame : public TFrame {
private:
    TMenu menuBar;
    TPopupMenu fileMenu;    
    TPopupMenu wndMenu;
    TToolBar* toolbar;
    TMdiCtrl* mdiCtrl;
    UINT childCount;
public:

    MyFrame() : TFrame("Test 1") {

        childCount = 0;

        addChild(toolbar = new TToolBar());
        addChild(mdiCtrl = new TMdiCtrl());

        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(ID_FILENEW, "New child", new TShortcut('N', FCONTROL));
        fileMenu.addItem(ID_FILEOPEN, "Open...", new TShortcut('O', FCONTROL));
        fileMenu.addSeparator();
        fileMenu.addItem(ID_EXIT, "Exit");
        menuBar.addPopupMenu(wndMenu, "Window");
        wndMenu.addItem(ID_CASCADE, "Cascade");
        wndMenu.addItem(ID_TILEHORIZONTAL, "Tile Horizontally");
        wndMenu.addItem(ID_TILEVERTICAL, "Tile Vertically");

        //fileMenu.setItemEnabled(ID_FILEOPEN, FALSE);
        setMenu(menuBar);
        mdiCtrl->setWndMenu(wndMenu);
    }
protected:
    void onSize(UINT width, UINT height) {
        TSize size = toolbar->resize();
        TRect rc(0, 0, width, height);
        rc.top += size.height;
        mdiCtrl->setBounds(rc);
    }

    void onCommand(UINT id, UINT code) {
        TStrBuffer str;
        
        switch(id) {
            case ID_CASCADE:
                mdiCtrl->cascade();
                break;

            case ID_TILEHORIZONTAL:
                mdiCtrl->tileHorizontally();
                break;

            case ID_TILEVERTICAL:
                mdiCtrl->tileVertically();
                break;

            case ID_FILENEW:
                {
                    str.format("Child #%d", ++childCount);
                    mdiCtrl->createChild(new MyChild(str.getBuffer()));
                }
                break;

            case ID_EXIT:
                close();
                break;

        }
    }

    void onCreate() {
        TFrame::onCreate();

        toolbar->addSeparator();
        toolbar->addStdButton(TToolBar::K_FILENEW, ID_FILENEW, "New");
        toolbar->addStdButton(TToolBar::K_FILEOPEN, ID_FILEOPEN, "Open");
        toolbar->addStdButton(TToolBar::K_FILESAVE, ID_FILESAVE, NULL, TToolBar::K_CHECK);
    }

    void handleEvent(TEvent& evt) {
        TFrame::handleEvent(evt);
        mdiCtrl->processDefault(evt);
    }

    BOOL canClose() {
        showMsg("Bye");
        return TRUE;
    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{ 
    TApplication app;
    MyFrame frame;

    return app.run(frame);
}
