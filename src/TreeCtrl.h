#ifndef TREECTRL_H
#define TREECTRL_H

#include "Window.h"
#include "ImageList.h"
#include "Vector.h"

class TreeCtrl;

class DllExport TreeNode {
public:
    TreeNode* addNode(LPSTR text, int bitmapIdx = -1);
    ~TreeNode();

    void setExpanded(BOOL isExpanded);
    TreeNode* getParent();
    TreeNode* getPrevSibling();
    TreeNode* getNextSibling();
    TreeNode* getFirstChild();
    void getNodePath(StrBuffer &path);

    void getText(StrBuffer& text);
    void setText(LPSTR strText);
    void clear();

private:
    TreeNode(LPSTR text, int bitmapIdx, TreeCtrl* pTreeCtrl, TreeNode* parent);

    void getNodeInfo(StrVector &vector);

    HTREEITEM hTreeItem;
    TreeCtrl* pTreeCtrl;
    Vector<TreeNode*> childs;

    friend class TreeCtrl;
};

class DllExport TreeCtrl : public Control {
public:
    TreeCtrl(UINT id, BOOL isEditable = FALSE);
    ~TreeCtrl();

    TreeNode* addNode(LPSTR text, int bitmapIdx = -1);
    void removeNode(TreeNode* pNode);

    TreeNode* getRootNode();
    TreeNode* getSelNode();
    TreeNode* getNodeAt(Point pt);
    UINT addBitmap(Bitmap* pBitmap);

protected:
    BOOL isDynamic;
    void onNotify(Event& evt);
private:
    TreeNode* getNode(HTREEITEM hItem);

    Vector<TreeNode*> childs;
    ImageList* pImageList;

    friend class TreeNode;

};

class DllExport DynamicTreeCtrl : public TreeCtrl {
public:
    DynamicTreeCtrl(UINT id);
protected:
    void onNotify(Event& evt);

    virtual BOOL onItemHasChildren(TreeNode* pNode) = 0;
    virtual void onItemExpanding(TreeNode* pNode) = 0;
};

#endif