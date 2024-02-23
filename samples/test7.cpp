
#include <windows.h>

#include "Application.h"
#include "Frame.h"

#include "TreeCtrl.h"
#include "File.h"
#include "StrVector.h"

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
        File::findFolder(path.getBuffer(), folders);
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
        ID_LIST1 = 100
    };

    TreeCtrl *tree1;

    MyFrame() : Frame("Test 7")
    {

        addChild(tree1 = new MyTree(ID_LIST1), Bounds(10, 40, 300, 300));
    }

protected:
    void onCommand(UINT id)
    {
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
