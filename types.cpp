#include "types.h"

TSize::TSize()
{
    this->width = 0;
    this->height = 0;
}

TSize::TSize(int width, int height)
{
    this->width = width;
    this->height = height;   
}

TBounds::TBounds()
{
    this->left = 0;
    this->top = 0;
}


TBounds::TBounds(int left, int top, int width, int height) : TSize(width, height)
{
    this->left = left;
    this->top = top;
}


TPoint::TPoint()
{
	x = 0;
	y = 0;
}

TPoint::TPoint(int x, int y)
{
	this->x = x;
    this->y = y;
}

TRect::TRect()
{
	SetRectEmpty(this);
}



TRect::TRect(int left, int top, int right, int bottom)
{
	SetRect(this, left, top, right, bottom);
}	

int TRect::getWidth()
{
	return abs(left - right);
}

int TRect::getHeight()
{
	return abs(top - bottom);
}

BOOL TRect::contains(int x, int y)
{
	POINT pt = {x, y};
	return PtInRect(this, pt);
}

TRect::TRect(TPoint p1, TPoint p2)
{
	SetRect(this, p1.x, p1.y, p2.x, p2.y);
}


void TRect::inflate(int dx, int dy)
{
	InflateRect(this, dx, dy);
}