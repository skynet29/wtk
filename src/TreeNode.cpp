#include "TreeNode.h"
#include "TreeCtrl.h"
#include "StrVector.h"

TreeNode::TreeNode(LPSTR text, int bitmapIdx, TreeCtrl *pTreeCtrl, TreeNode *parent)
{
	this->pTreeCtrl = pTreeCtrl;

	TV_INSERTSTRUCT tvins;
	TV_ITEM &tvi = tvins.item;

	tvins.hParent = (parent) ? parent->hTreeItem : NULL;
	tvins.hInsertAfter = TVI_LAST;

	tvi.mask = TVIF_TEXT | TVIF_PARAM;
	tvi.pszText = text;
	tvi.cchTextMax = strlen(text);
	tvi.lParam = (LPARAM)this;
	if (pTreeCtrl->isDynamic) {		
		tvi.mask |= TVIF_CHILDREN;
		tvi.cChildren = I_CHILDRENCALLBACK;
	}
		
	if (bitmapIdx >= 0)
	{
		tvi.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.iImage = tvi.iSelectedImage = bitmapIdx;
	}

	hTreeItem = TreeView_InsertItem(pTreeCtrl->getHandle(), &tvins);
}

void TreeNode::clear()
{
	for (UINT idx = 0; idx < childs.getCount(); idx++)
	{
		delete childs[idx];
	}
}

TreeNode::~TreeNode()
{
	clear();
}

void TreeNode::getText(StrBuffer &text)
{
	static char buffer[64];
	TV_ITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.hItem = hTreeItem;
	tvi.pszText = buffer;
	tvi.cchTextMax = sizeof(buffer);
	TreeView_GetItem(pTreeCtrl->getHandle(), &tvi);
	text.set(tvi.pszText);
}

void TreeNode::setText(LPSTR strText)
{
	TV_ITEM tvi;

	tvi.mask = TVIF_TEXT;
	tvi.pszText = strText;
	tvi.cchTextMax = strlen(strText);
	tvi.hItem = hTreeItem;
	TreeView_SetItem(pTreeCtrl->getHandle(), &tvi);
}

TreeNode *TreeNode::addNode(LPSTR text, int bitmapIdx)
{
	return new TreeNode(text, bitmapIdx, pTreeCtrl, this);
}

void TreeNode::setExpanded(BOOL isExpanded)
{
	TreeView_Expand(pTreeCtrl->getHandle(), hTreeItem,
					(isExpanded) ? TVE_EXPAND : TVE_COLLAPSE);
}

TreeNode *TreeNode::getParent()
{
	HTREEITEM hItem = TreeView_GetParent(pTreeCtrl->getHandle(), hTreeItem);
	return (hItem != NULL) ? pTreeCtrl->getNode(hItem) : NULL;
}

TreeNode *TreeNode::getFirstChild()
{
	HTREEITEM hItem = TreeView_GetChild(pTreeCtrl->getHandle(), hTreeItem);
	return (hItem != NULL) ? pTreeCtrl->getNode(hItem) : NULL;
}

TreeNode *TreeNode::getNextSibling()
{
	HTREEITEM hItem = TreeView_GetNextSibling(pTreeCtrl->getHandle(), hTreeItem);
	return (hItem != NULL) ? pTreeCtrl->getNode(hItem) : NULL;
}

TreeNode *TreeNode::getPrevSibling()
{
	HTREEITEM hItem = TreeView_GetPrevSibling(pTreeCtrl->getHandle(), hTreeItem);
	return (hItem != NULL) ? pTreeCtrl->getNode(hItem) : NULL;
}

void TreeNode::getNodeInfo(StrVector &vector)
{
	StrBuffer text;
	getText(text);
	vector.add(text.getBuffer());
	TreeNode *pParent = getParent();
	if (pParent != NULL)
		pParent->getNodeInfo(vector);
}

void TreeNode::getNodePath(StrBuffer &path)
{
	StrVector v;
	getNodeInfo(v);
	for (int i = v.getCount() - 1; i >= 0; i--)
	{
		path.append(v[i]);
		path.append("\\");
	}
}