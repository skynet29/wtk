#include "ScrollBar.h"

ScrollBar::ScrollBar(HWND hWnd, UINT range, UINT inc, UINT type)
{
	this->inc = inc;
	this->type = type;
	this->hWnd = hWnd;

	Rect rc;
	GetClientRect(hWnd, &rc);

	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nPos = 0;
	info.nMax = range;
	info.nPage = (type == SB_VERT) ? rc.getHeight() : rc.getWidth();

	SetScrollInfo(hWnd, type, &info, FALSE);
}

void ScrollBar::resize(UINT newSize)
{
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_PAGE;
	info.nPage = newSize;
	SetScrollInfo(hWnd, type, &info, TRUE);
}

void ScrollBar::scroll(UINT code, UINT trackPos)
{
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_ALL;
	GetScrollInfo(hWnd, type, &info);
	int nPos = info.nPos;

	switch (code)
	{
	case SB_LINEUP:
		nPos -= inc;
		break;

	case SB_LINEDOWN:
		nPos += inc;
		break;

	case SB_PAGEUP:
		nPos -= info.nPage;
		break;

	case SB_PAGEDOWN:
		nPos += info.nPage;
		break;

	case SB_THUMBTRACK:
		nPos = trackPos;
		break;
	}

	nPos = min(info.nMax - (int)info.nPage, max(info.nMin, nPos));

	SetScrollPos(hWnd, type, nPos, TRUE);

	if (type == SB_VERT)
		ScrollWindow(hWnd, 0, info.nPos - nPos, NULL, NULL);
	else
		ScrollWindow(hWnd, info.nPos - nPos, 0, NULL, NULL);
}

UINT ScrollBar::getPos()
{
	return GetScrollPos(hWnd, type);
}

void ScrollBar::setPos(UINT pos)
{
	SetScrollPos(hWnd, type, pos, TRUE);
}