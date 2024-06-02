#ifndef TREENODE_H
#define TREENODE_H

#include "Vector.h"
#include "StrBuffer.h"

class TreeCtrl;
class TreeNode;

class DllExport BaseNode {
public:
    ~BaseNode();
    void addNode(TreeNode* pNode);
    TreeNode* addNode(LPSTR text, int bitmapIdx = -1);
    void clear();

    void insertAt(TreeNode *pNode, UINT idx);
    UINT getChildCount();
    TreeNode* getChildAt(UINT idx);

protected:
    virtual void createNode(TreeNode* pNode, HTREEITEM hInsertAfter) = 0;
    Vector<TreeNode*> childs;

};

class DllExport TreeNode : public BaseNode {
public:
    TreeNode(LPSTR text, int bitmapIdx = -1);

    void setExpanded(BOOL isExpanded);
    TreeNode* getParent();
    TreeNode* getPrevSibling();
    TreeNode* getNextSibling();
    TreeNode* getFirstChild();
    void getNodePath(StrBuffer &path);
    UINT getIndex();
    LPSTR getText();
    void setText(LPSTR strText);
    void select();
    void remove();
    void moveUp();
    void moveDown();
    BOOL isLast();
    void editLabel(); 
    HTREEITEM getHandle() {return hTreeItem;}

private:
    
    void createNode(TreeNode* pNode, HTREEITEM hInsertAfter);
    void create(TreeCtrl* pTreeCtrl, HTREEITEM hParent, HTREEITEM hInsertAfter);

    void getNodeInfo(StrVector &vector);

    HTREEITEM hTreeItem;
    TreeCtrl* pTreeCtrl;
    StrBuffer text;
    int bitmapIdx;

    friend class TreeCtrl;
};

#endif