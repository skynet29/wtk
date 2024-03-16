#include "TreeCtrl.h"
#include "Container.h"

TreeCtrl::TreeCtrl(BOOL isEditable)
{
	attr.className = WC_TREEVIEW;
	attr.styleEx = WS_EX_CLIENTEDGE;
	attr.style = WS_VISIBLE | WS_CHILD |
				 TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	if (isEditable)
		attr.style |= TVS_EDITLABELS;

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

void TreeCtrl::addNode(TreeNode* pNode)
{
	pNode->create(this, NULL, TVI_LAST);
	childs.add(pNode);
}

TreeNode* TreeCtrl::addNode(LPSTR text, int bitmapIdx)
{
	TreeNode* pNode = new TreeNode(text, bitmapIdx);
	addNode(pNode);
	return pNode;
}

void TreeCtrl::insertAt(TreeNode *pNode, UINT idx)
{
	childs.insertAt(pNode, idx);
	pNode->create(this, NULL,
			(idx == 0) ? TVI_FIRST : childs[idx-1]->hTreeItem);
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
		onSelChange.fire(this);
		break;

	case NM_RCLICK:
	{
		onRightClick.fire(this);
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

UINT TreeCtrl::getChildCount()
{
	return childs.getCount();
}

TreeNode* TreeCtrl::getChildAt(UINT idx)
{
	return childs[idx];
}

//////////////////////

DynamicTreeCtrl::DynamicTreeCtrl()
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

