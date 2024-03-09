#ifndef TREENODE_H
#define TREENODE_H

#include "Vector.h"
#include "StrBuffer.h"

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

#endif