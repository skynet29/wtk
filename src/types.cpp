#include "types.h"
#include <stdio.h>

void debugPrint(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[1024];
	vsprintf(buffer, fmt, args);
	va_end(args);
	DWORD dwBytesWritten;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwBytesWritten, NULL);
}

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

Bounds::Bounds(Point p1, Point p2)
{
	left = min(p1.x, p2.x);
	top = min(p1.y, p2.y);
	width = abs(p1.x - p2.x);
	height = abs(p1.y - p2.y);
}

Rect Bounds::toRect()
{
	return Rect(left, top, left + width, top + height);
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

BOOL Rect::contains(Point pt)
{
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