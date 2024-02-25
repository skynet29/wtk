#include "FolderTreeCtrl.h"
#include "File.h"

FolderTreeCtrl::FolderTreeCtrl(UINT id, LPSTR strPath) : DynamicTreeCtrl(id)
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

void FolderTreeCtrl::getFolder(TreeNode *pNode, StrVector &folders)
{
    StrBuffer path(strPath.getBuffer());
    path.append("\\");
    pNode->getNodePath(path);
    File::findFolder(path, folders);
}

BOOL FolderTreeCtrl::onItemHasChildren(TreeNode *pNode)
{
    StrVector folders;
    getFolder(pNode, folders);
    return (folders.getCount() > 0);
}

void FolderTreeCtrl::onItemExpanding(TreeNode *pNode)
{
    StrVector folders;
    getFolder(pNode, folders);

    for (UINT i = 0; i < folders.getCount(); i++)
    {
        pNode->addNode(folders[i]);
    }
}