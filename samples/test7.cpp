
#include <windows.h>

#include "Application.h"
#include "Frame.h"

#include "TreeCtrl.h"
#include "File.h"
#include "StrVector.h"
#include "Layout.h"
#include "TextField.h"
#include "Button.h"

class MyTree : public DynamicTreeCtrl
{
public:
    MyTree(UINT id) : DynamicTreeCtrl(id) {}

protected:
    void getFolder(TreeNode* pNode, StrVector& folders) {
        StrBuffer path(getenv("USERPROFILE"));
        path.append("\\");
        pNode->getNodePath(path);
        //printf("path=%s\n", path.getBuffer());
        File::findFolder(path, folders);
    }

    BOOL onItemHasChildren(TreeNode *pNode)
    {
        StrVector folders;
        getFolder(pNode, folders);
        return (folders.getCount() > 0);
    }
    void onItemExpanding(TreeNode *pNode)
    {
        printf("onItemExpanding\n");

        StrVector folders;
        getFolder(pNode, folders);

        for (UINT i = 0; i < folders.getCount(); i++)
        {
            pNode->addNode(folders[i]);
        }
    }
};

class MyFrame : public Frame
{
private:
public:
    enum
    {
        ID_TREE1 = 100
    };

    TreeCtrl *tree1;
    TextField* text1;
    Container* cont1;

    MyFrame() : Frame("Test 7")
    {
        cont1 = new Container();
        cont1->setBackColor(Color::getSysColor());

        Layout layout2(cont1, 0, 0);
        layout2.add(new Button("OK", IDOK), Size(60, 25));
        layout2.add(new Button("Cancel", IDCANCEL), Size(60, 25), 10);

        Layout layout(this, 10, 10);
        layout.add(tree1 = new MyTree(ID_TREE1), Size(500, 400));
        layout.endl();
        layout.addLabel("Folder:", 0, 25);
        layout.addEnd(text1 = new TextField(0), 25, 5);
        layout.endl();
        layout.addRight(cont1, cont1->getPackSize());



        setBackColor(Color::getSysColor());
    }

protected:
    void onCommand(UINT id)
    {
    }

    void onSelChange(UINT id) {
        if (id == ID_TREE1) {
            StrBuffer text(getenv("USERPROFILE"));
            text.append("\\");
            tree1->getSelNode()->getNodePath(text);
            text1->setText(text);
        }
    }

    void onCreate()
    {
        Frame::onCreate();

        StrVector folders;
        File::findFolder(getenv("USERPROFILE"), folders);
        for (UINT i = 0; i < folders.getCount(); i++)
        {
            tree1->addNode(folders[i]);
        }
        packSize(10, 10);
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
