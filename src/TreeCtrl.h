#ifndef TREECTRL_H
#define TREECTRL_H

#include "Control.h"
#include "ImageList.h"
#include "TreeNode.h"



class DllExport TreeCtrl : public Control, public BaseNode {
public:
    TreeCtrl(BOOL isEditable = FALSE);
    ~TreeCtrl();

    TreeNode* getRootNode();
    TreeNode* getSelNode();
    TreeNode* getNodeAt(Point pt);
    UINT addBitmap(Bitmap* pBitmap);
    void setOnSelChange(Callback* cbk) {onSelChange.set(cbk);}
    void setOnRightClick(Callback* cbk) {onRightClick.set(cbk);}

protected:
    BOOL isDynamic;
    void onNotify(Event& evt);
private:
    void createNode(TreeNode* pNode, HTREEITEM hInsertAfter);
    TreeNode* getNode(HTREEITEM hItem);
    CbkHolder onSelChange;
    CbkHolder onRightClick;

    ImageList* pImageList;

    friend class TreeNode;

};

class DllExport DynamicTreeCtrl : public TreeCtrl {
public:
    DynamicTreeCtrl();
protected:
    void onNotify(Event& evt);

    virtual BOOL onItemHasChildren(TreeNode* pNode) = 0;
    virtual void onItemExpanding(TreeNode* pNode) = 0;
};

#endif