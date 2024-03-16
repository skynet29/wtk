#ifndef TREENODE_H
#define TREENODE_H

#include "Vector.h"
#include "StrBuffer.h"

class TreeCtrl;

class DllExport TreeNode {
public:
    TreeNode(LPSTR text, int bitmapIdx = -1);

    void addNode(TreeNode* pNode);
    TreeNode* addNode(LPSTR text, int bitmapIdx = -1);

    void insertAt(TreeNode *pNode, UINT idx);

    ~TreeNode();

    void setExpanded(BOOL isExpanded);
    TreeNode* getParent();
    TreeNode* getPrevSibling();
    TreeNode* getNextSibling();
    TreeNode* getFirstChild();
    void getNodePath(StrBuffer &path);
    UINT getIndex();
    UINT getChildCount();
    TreeNode* getChildAt(UINT idx);
    LPSTR getText();
    void setText(LPSTR strText);
    void clear();
    void select();
    void remove();
    void moveUp();
    void moveDown();



private:
    
    
    void create(TreeCtrl* pTreeCtrl, HTREEITEM hParent, HTREEITEM hInsertAfter);

    void getNodeInfo(StrVector &vector);

    HTREEITEM hTreeItem;
    TreeCtrl* pTreeCtrl;
    StrBuffer text;
    int bitmapIdx;
    Vector<TreeNode*> childs;

    friend class TreeCtrl;
};

#endif