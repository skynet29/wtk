#ifndef FOLDERTREECTRL_H
#define FOLDERTREECTRL_H

#include "TreeCtrl.h"
#include "StrVector.h"

class DllExport FolderTreeCtrl : public DynamicTreeCtrl {
public: 
    FolderTreeCtrl(LPSTR strPath);

    void create(HWND hParent);

protected:
    StrBuffer strPath;
    void getFolder(TreeNode* pNode, StrVector& folders);

    BOOL onItemHasChildren(TreeNode *pNode);
    void onItemExpanding(TreeNode *pNode);
};

#endif