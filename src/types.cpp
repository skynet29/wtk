#include "types.h"

Size::Size()
{
    this->width = 0;
    this->height = 0;
}

Size::Size(int width, int height)
{
    this->width = width;
    this->height = height;   
}

Bounds::Bounds()
{
    this->left = 0;
    this->top = 0;
}


Bounds::Bounds(int left, int top, int width, int height) : Size(width, height)
{
    this->left = left;
    this->top = top;
}


Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(int x, int y)
{
	this->x = x;
    this->y = y;
}

Rect::Rect()
{
	SetRectEmpty(this);
}



Rect::Rect(int left, int top, int right, int bottom)
{
	SetRect(this, left, top, right, bottom);
}	

int Rect::getWidth()
{
	return abs(left - right);
}

int Rect::getHeight()
{
	return abs(top - bottom);
}

BOOL Rect::contains(int x, int y)
{
	POINT pt = {x, y};
	return PtInRect(this, pt);
}

Rect::Rect(Point p1, Point p2)
{
	SetRect(this, p1.x, p1.y, p2.x, p2.y);
}


void Rect::inflate(int dx, int dy)
{
	InflateRect(this, dx, dy);
}