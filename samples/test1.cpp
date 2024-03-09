

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "ToolBar.h"
#include "MdiCtrl.h"
#include "TextArea.h"
#include "File.h"
#include "Icon.h"

enum
{
    ID_FILEOPEN = 200,
    ID_FILENEW,
    ID_FILESAVE,
    ID_CASCADE,
    ID_TILEVERTICAL,
    ID_TILEHORIZONTAL,
    ID_EXIT
};

class MyChild : public MdiChild
{
private:
    TextArea *pTextArea;
    Font* pFont;
public:
    MyChild(LPSTR title) : MdiChild(title)
    {
        addChild(pTextArea = new TextArea());
        pFont = new Font("Courier New", 12);

    }

protected:
    void onSize(UINT width, UINT height)
    {
        pTextArea->setSize(width, height);
    }

    void onCreate() {
        MdiChild::onCreate();

        pTextArea->setFont(pFont);
    }

    void onFocus()
    {
        pTextArea->setFocus();
    }

    friend class MyFrame;
};

class MyFrame : public Frame
{
private:
    Menu menuBar;
    PopupMenu fileMenu;
    PopupMenu wndMenu;
    ToolBar *toolbar;
    MdiCtrl *mdiCtrl;
    UINT childCount;

public:
    MyFrame() : Frame("Test 1")
    {

        childCount = 0;

        addChild(toolbar = new ToolBar());
        addChild(mdiCtrl = new MdiCtrl());

        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(ID_FILENEW, "New child", new Shortcut('N', FCONTROL));
        fileMenu.addItem(ID_FILEOPEN, "Open...", new Shortcut('O', FCONTROL));
        fileMenu.addSeparator();
        fileMenu.addItem(ID_EXIT, "Exit");
        Icon* pIcon = Icon::createFromBitmap("bitmaps\\open.bmp", Color::GREEN);
        debugPrint("pIcon=%p\n", pIcon);
        fileMenu.setItemIcon(ID_FILEOPEN, pIcon);
        pIcon = Icon::createFromBitmap("bitmaps\\new.bmp", Color::GREEN);
        fileMenu.setItemIcon(ID_FILENEW, pIcon);

        menuBar.addPopupMenu(wndMenu, "Window");
        wndMenu.addItem(ID_CASCADE, "Cascade");
        pIcon = Icon::createFromBitmap("bitmaps\\cascade.bmp", Color::GREEN);
        wndMenu.setItemIcon(ID_CASCADE, pIcon);

        wndMenu.addItem(ID_TILEHORIZONTAL, "Tile Horizontally");
        pIcon = Icon::createFromBitmap("bitmaps\\tile_horz.bmp", Color::GREEN);
        wndMenu.setItemIcon(ID_TILEHORIZONTAL, pIcon);

        wndMenu.addItem(ID_TILEVERTICAL, "Tile Vertically");
        pIcon = Icon::createFromBitmap("bitmaps\\tile_vert.bmp", Color::GREEN);
        wndMenu.setItemIcon(ID_TILEVERTICAL, pIcon);
        // fileMenu.setItemEnabled(ID_FILEOPEN, FALSE);
        setMenu(menuBar);
        mdiCtrl->setWndMenu(wndMenu);
    }

protected:
    void onSize(UINT width, UINT height)
    {
        Size size = toolbar->resize();
        Rect rc(0, 0, width, height);
        rc.top += size.height;
        mdiCtrl->setBounds(rc);
    }

    void onCommand(UINT id)
    {
        StrBuffer str;

        switch (id)
        {
        case ID_CASCADE:
            mdiCtrl->cascade();
            break;

        case ID_TILEHORIZONTAL:
            mdiCtrl->tileHorizontally();
            break;

        case ID_TILEVERTICAL:
            mdiCtrl->tileVertically();
            break;

        case ID_FILEOPEN:
        {
            LPSTR fileName = getOpenFileName("Text|*.txt|Javascript|*.js");
            if (fileName != NULL)
            {
                printf("fileName=%s\n", fileName);
                StrBuffer text;
                if (File::readTextFile(fileName, text))
                {
                    MyChild *pChild = new MyChild(fileName);
                    mdiCtrl->createChild(pChild);
                    pChild->pTextArea->setText(text.getBuffer());
                }
                else {
                    printf("fail to read file\n");
                }
            }
        }
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

    void onCreate()
    {
        Frame::onCreate();

        toolbar->addSeparator();
        toolbar->addStdButton(ToolBar::K_FILENEW, ID_FILENEW, "New");
        toolbar->addStdButton(ToolBar::K_FILEOPEN, ID_FILEOPEN, "Open");
        toolbar->addStdButton(ToolBar::K_FILESAVE, ID_FILESAVE, NULL, ToolBar::K_CHECK);
    }

    void handleEvent(Event &evt)
    {
        Frame::handleEvent(evt);
        mdiCtrl->processDefault(evt);
    }

    BOOL canClose()
    {
        showMsg("Bye");
        return TRUE;
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
