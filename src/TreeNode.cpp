#include "TreeNode.h"
#include "TreeCtrl.h"
#include "StrVector.h"


void BaseNode::clear()
{
	for (UINT idx = 0; idx < childs.getCount(); idx++)
	{
		delete childs[idx];
	}
}

BaseNode::~BaseNode()
{
	clear();
}

void BaseNode::addNode(TreeNode* pNode)
{
	createNode(pNode, TVI_LAST);
	childs.add(pNode);
}

TreeNode* BaseNode::addNode(LPSTR text, int bitmapIdx)
{
	TreeNode* pNode = new TreeNode(text, bitmapIdx);
	addNode(pNode);
	return pNode;
}

void BaseNode::insertAt(TreeNode *pNode, UINT idx)
{
	childs.insertAt(pNode, idx);
	createNode(pNode, (idx == 0) ? TVI_FIRST : childs[idx-1]->getHandle());
}

UINT BaseNode::getChildCount()
{
	return childs.getCount();
}

TreeNode* BaseNode::getChildAt(UINT idx)
{
	return childs[idx];
}

//////////////////////////
TreeNode::TreeNode(LPSTR text, int bitmapIdx)
{
	this->text.set(text);
	this->bitmapIdx = bitmapIdx;
}

void TreeNode::createNode(TreeNode* pNode, HTREEITEM hInsertAfter)
{
	pNode->create(pTreeCtrl, hTreeItem, hInsertAfter);
}

void TreeNode::create(TreeCtrl *pTreeCtrl, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	this->pTreeCtrl = pTreeCtrl;

	TV_INSERTSTRUCT tvins;
	TV_ITEM &tvi = tvins.item;

	tvins.hParent = hParent;
	tvins.hInsertAfter = hInsertAfter;

	tvi.mask = TVIF_TEXT | TVIF_PARAM;
	tvi.pszText = text.getBuffer();
	tvi.cchTextMax = text.getLength();
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



LPSTR TreeNode::getText()
{
	return text.getBuffer();
}

void TreeNode::setText(LPSTR strText)
{
	TV_ITEM tvi;

	tvi.mask = TVIF_TEXT;
	tvi.pszText = strText;
	tvi.cchTextMax = strlen(strText);
	tvi.hItem = hTreeItem;
	TreeView_SetItem(pTreeCtrl->getHandle(), &tvi);
	text.set(strText);
}



void TreeNode::setExpanded(BOOL isExpanded)
{
	TreeView_Expand(pTreeCtrl->getHandle(), hTreeItem,
					(isExpanded) ? TVE_EXPAND : TVE_COLLAPSE);
}

void TreeNode::select()
{
	TreeView_SelectItem(pTreeCtrl->getHandle(), hTreeItem);
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
	vector.add(getText());
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

UINT TreeNode::getIndex()
{
	TreeNode* parent = getParent();
	if (parent != NULL)
		return parent->childs.find(this);

	return pTreeCtrl->childs.find(this);
}



void TreeNode::remove()
{
	TreeNode *parent = getParent();

	if (parent != NULL)
		parent->childs.remove(this);
	else
		pTreeCtrl->childs.remove(this);

	pTreeCtrl->sendMsg(TVM_DELETEITEM, 0, (LPARAM)hTreeItem);
}

void TreeNode::moveUp()
{
	UINT idx = getIndex();
	if (idx > 0)
	{
		TreeNode* parent = getParent();
		remove();
		if (parent != NULL)
			parent->insertAt(this, idx-1);	
		else
			pTreeCtrl->insertAt(this, idx-1);
	}
}

BOOL TreeNode::isLast() 
{
	UINT idx = getIndex();
	TreeNode* parent = getParent();
	UINT childCount = (parent != NULL) ? parent->getChildCount() : pTreeCtrl->getChildCount();
	return (idx == childCount-1);	
}

void TreeNode::moveDown()
{
	if (!isLast())
	{
		UINT idx = getIndex();
		TreeNode* parent = getParent();

		remove();
		if (parent != NULL)
			parent->insertAt(this, idx+1);	
		else
			pTreeCtrl->insertAt(this, idx+1);
	}
}

void TreeNode::editLabel()
{
	TreeView_EditLabel(pTreeCtrl->getHandle(), hTreeItem);
}