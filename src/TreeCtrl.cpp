#include "TreeCtrl.h"
#include "Container.h"
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
/////////////////////////////////

TreeCtrl::TreeCtrl(UINT id, BOOL isEditable)
{
	attr.className = WC_TREEVIEW;
	attr.styleEx = WS_EX_CLIENTEDGE;
	attr.style = WS_VISIBLE | WS_CHILD |
				 TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	if (isEditable)
		attr.style |= TVS_EDITLABELS;

	attr.hMenu = (HMENU)id;

	isDynamic = FALSE;

	pImageList = NULL;
}

TreeCtrl::~TreeCtrl()
{
	if (pImageList != NULL)
		delete pImageList;

	for (UINT idx = 0; idx < childs.getCount(); idx++)
	{
		delete childs[idx];
	}
}

UINT TreeCtrl::addBitmap(Bitmap *pBitmap)
{
	if (pImageList == NULL)
	{
		pImageList = new ImageList(16, 16);

		TreeView_SetImageList(hWnd, pImageList->getHandle(), TVSIL_NORMAL);
	}

	return pImageList->addBitmap(pBitmap);
}

TreeNode *TreeCtrl::addNode(LPSTR text, int bitmapIdx)
{
	return new TreeNode(text, bitmapIdx, this, NULL);
}

void TreeCtrl::removeNode(TreeNode *pNode)
{
	TreeNode *parent = pNode->getParent();

	if (parent != NULL)
		parent->childs.remove(pNode);
	else
		childs.remove(pNode);

	sendMsg(TVM_DELETEITEM, 0, (LPARAM)pNode->hTreeItem);
}

TreeNode *TreeCtrl::getNode(HTREEITEM hItem)
{
	TV_ITEM tvi;
	tvi.mask = TVIF_PARAM;
	tvi.hItem = hItem;
	TreeView_GetItem(hWnd, &tvi);
	return (TreeNode *)tvi.lParam;
}

TreeNode *TreeCtrl::getRootNode()
{
	HTREEITEM hItem = TreeView_GetRoot(hWnd);
	return (hItem != NULL) ? getNode(hItem) : NULL;
}

TreeNode *TreeCtrl::getSelNode()
{
	HTREEITEM hItem = TreeView_GetSelection(hWnd);
	return (hItem != NULL) ? getNode(hItem) : NULL;
}

void TreeCtrl::onNotify(Event &evt)
{

	LPNMHDR lpHeader = (LPNMHDR)evt.lParam;

	switch (lpHeader->code)
	{
	case TVN_SELCHANGED:
		parent->onSelChange(getId());
		break;

	case NM_RCLICK:
	{
		Point pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);
		parent->onRightClick(getId(), pt);
	}
	break;

	case TVN_ENDLABELEDIT:
	{
		LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lpHeader;
		LPSTR strText = ptvdi->item.pszText;
		if (strText != NULL)
		{
			TreeNode *pNode = (TreeNode *)ptvdi->item.lParam;
			if (pNode != NULL)
				pNode->setText(strText);
		}
	}
	break;

	default:
		break;
	}
}

TreeNode *TreeCtrl::getNodeAt(Point pt)
{
	TVHITTESTINFO ht;
	ht.pt = pt;

	TreeView_HitTest(hWnd, &ht);

	if (ht.hItem)
	{
		Rect rcItem;
		TreeView_GetItemRect(hWnd, ht.hItem, &rcItem, TRUE);

		if (rcItem.contains(pt))
			return getNode(ht.hItem);
	}
	return NULL;
}

//////////////////////

DynamicTreeCtrl::DynamicTreeCtrl(UINT id) : TreeCtrl(id) 
{
	isDynamic = TRUE;
}

void DynamicTreeCtrl::onNotify(Event& evt)
{
	
	LPNMHDR lpHeader = (LPNMHDR)evt.lParam;

	switch (lpHeader->code)
	{

	case TVN_GETDISPINFO:
		{
			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lpHeader;
			TVITEM& tvi = ptvdi->item;
			TreeNode* pNode = (TreeNode*)tvi.lParam;
			if ((tvi.mask & TVIF_CHILDREN) != 0) {
				tvi.cChildren = onItemHasChildren(pNode);
			}
		}	
		break;

	case TVN_ITEMEXPANDED:
       {
			LPNMTREEVIEW lptv = (LPNMTREEVIEW)evt.lParam;
			TVITEM& item = lptv->itemNew;

           // Do a TVE_COLLAPSERESET on the parent to minimize memory use.

           if (lptv->action == TVE_COLLAPSE)
           {
				TreeNode* pNode = (TreeNode*)item.lParam;

                TreeView_Expand (hWnd,
                                 item.hItem,
                                 TVE_COLLAPSE | TVE_COLLAPSERESET);

				pNode->clear();
           }
	   }
	   break;


	case TVN_ITEMEXPANDING: 
		{	
			LPNMTREEVIEW lptv = (LPNMTREEVIEW)evt.lParam;
			TVITEM& item = lptv->itemNew;
			if (lptv->action == TVE_EXPAND) {
				TreeNode* pNode = (TreeNode*)item.lParam;
				if (pNode != NULL) {
					onItemExpanding(pNode);
				}
			}
		}
		break;		

	default:
		TreeCtrl::onNotify(evt);
		break;
	}
}