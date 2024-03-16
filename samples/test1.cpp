

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "ToolBar.h"
#include "MdiCtrl.h"
#include "TextArea.h"
#include "File.h"
#include "Icon.h"


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
    Icon* pFileNewIcon;
    Icon* pFileOpenIcon;
    Icon* pFileSaveIcon;

public:
    MyFrame() : Frame("Test 1")
    {

        childCount = 0;

        addChild(toolbar = new ToolBar());
        addChild(mdiCtrl = new MdiCtrl());

        pFileNewIcon = Icon::createFromBitmap("bitmaps\\new.bmp", Color::GREEN);
        pFileOpenIcon = Icon::createFromBitmap("bitmaps\\open.bmp", Color::GREEN);
        pFileSaveIcon = Icon::createFromBitmap("bitmaps\\save.bmp", Color::GREEN);

        MenuItem *pItem;
        menuBar.addPopupMenu(fileMenu, "File");

        pItem = fileMenu.addItem("New child", new Shortcut('N', FCONTROL));
        pItem->setOnClick(CBK(MyFrame, newChild));
        pItem->setIcon(pFileNewIcon);

        pItem = fileMenu.addItem("Open...", new Shortcut('O', FCONTROL));
        pItem->setOnClick(CBK(MyFrame, openFile));
        pItem->setIcon(pFileOpenIcon);

        fileMenu.addSeparator();

        pItem = fileMenu.addItem("Exit");
        pItem->setOnClick(CBK(MyFrame, exit));
        

        menuBar.addPopupMenu(wndMenu, "Window");

        pItem = wndMenu.addItem("Cascade");
        pItem->setOnClick(CBK(MyFrame, cascade));
        pItem->setIcon(Icon::createFromBitmap("bitmaps\\cascade.bmp", Color::GREEN));

        pItem = wndMenu.addItem("Tile Horizontally");
        pItem->setOnClick(CBK(MyFrame, tileHorizontally));
        pItem->setIcon(Icon::createFromBitmap("bitmaps\\tile_horz.bmp", Color::GREEN));

        pItem = wndMenu.addItem("Tile Vertically");
        pItem->setOnClick(CBK(MyFrame, tileVertically));
        pItem->setIcon(Icon::createFromBitmap("bitmaps\\tile_vert.bmp", Color::GREEN));
        
        setMenu(menuBar);
        mdiCtrl->setWndMenu(wndMenu);
    }

protected:
    void exit(void* from) {
        close();
    }
    void newChild(void* from) {
        StrBuffer str;
        str.format("Child #%d", ++childCount);
        mdiCtrl->createChild(new MyChild(str.getBuffer()));
    }

    void openFile(void* from) {
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
    void onSize(UINT width, UINT height)
    {
        Size size = toolbar->resize();
        Rect rc(0, 0, width, height);
        rc.top += size.height;
        mdiCtrl->setBounds(rc);
    }

    void cascade(void* from) {
        mdiCtrl->cascade();
    }

    void tileHorizontally(void* from) {
        mdiCtrl->tileHorizontally();
    }

    void tileVertically(void* from) {
         mdiCtrl->tileVertically();
    }

    void onCreate()
    {
        Frame::onCreate();

        toolbar->addSeparator();
        ToolButton* pFileNew = toolbar->addButton(pFileNewIcon);
        pFileNew->setOnClick(CBK(MyFrame, newChild));
        pFileNew->setToolTips("New");
        ToolButton* pFileOpen = toolbar->addButton(pFileOpenIcon);
        pFileOpen->setOnClick(CBK(MyFrame, openFile));
        pFileOpen->setToolTips("Open");
        toolbar->addButton(pFileSaveIcon, ToolBar::K_CHECK);

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
