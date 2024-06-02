#include "FolderTreeCtrl.h"
#include "File.h"

FolderTreeCtrl::FolderTreeCtrl(LPSTR strPath)
{
    this->strPath.set(strPath);
}

void FolderTreeCtrl::create(HWND hParent)
{
    DynamicTreeCtrl::create(hParent);

    StrVector folders;
    File::findFolder(strPath, folders);
    for (UINT i = 0; i < folders.getCount(); i++)
    {
        addNode(folders[i]);
    }
}


BOOL FolderTreeCtrl::onItemHasChildren(TreeNode *pNode)
{
    StrBuffer path(strPath.getBuffer());
    path.append("\\");
    pNode->getNodePath(path);
    return File::hasFolder(path);
}

void FolderTreeCtrl::onItemExpanding(TreeNode *pNode)
{
    StrVector folders;
    StrBuffer path(strPath.getBuffer());
    path.append("\\");
    pNode->getNodePath(path);
    File::findFolder(path, folders);

    for (UINT i = 0; i < folders.getCount(); i++)
    {
        pNode->addNode(folders[i]);
    }
}