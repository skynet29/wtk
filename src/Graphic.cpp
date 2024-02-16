#include "Graphic.h"


Graphic::Graphic(HDC hDC)
{
    this->hDC = hDC;
    this->hPen = NULL;
    this->hBrush = NULL;

    SetBkMode(hDC, TRANSPARENT);
}

void Graphic::setDrawMode(DrawMode mode)
{
    switch(mode) {
        case K_NORMAL:
        	SetROP2(hDC, R2_COPYPEN);
            break;

        case K_NOT:
            SetROP2(hDC, R2_NOT);
            break;

        case K_XOR:
            SetROP2(hDC, R2_XORPEN);
            break;

    }
}


void Graphic::drawRect(Bounds bounds)
{
    Rectangle(hDC, bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
}

void Graphic::drawRect(Point p1, Point p2)
{
    Rectangle(hDC, p1.x, p1.y, p2.x, p2.y);
}

void Graphic::drawEllipse(Bounds bounds)
{
    Ellipse(hDC, bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
}

void Graphic::drawCircle(Point center, UINT radius)
{
    drawEllipse(Bounds(center.x - radius, center.y - radius, radius * 2, radius * 2));
}

void Graphic::setBrush(Color color)
{
    hBrush = CreateSolidBrush(color);
    DeleteObject(SelectObject(hDC, hBrush));
}

void Graphic::setPen(Color color, UINT width, UINT style)
{
    hPen = CreatePen(style, width, color);
    DeleteObject(SelectObject(hDC, hPen));
}

void Graphic::useHollowBrush() {
    DeleteObject(SelectObject(hDC, GetStockObject(HOLLOW_BRUSH)));
}

void Graphic::useHollowPen() {
    DeleteObject(SelectObject(hDC, GetStockObject(NULL_PEN)));
}

Graphic::~Graphic() {
    if (hPen != NULL) {
        DeleteObject(hPen);
    }
    if (hBrush != NULL) {
        DeleteObject(hBrush);
    }    
}

void Graphic::drawBitmap(Point pt, Bitmap* pBitmap)
{
	HBITMAP hBitmap = pBitmap->getHandle();
	if (hBitmap)
	{
		Size bmSize = pBitmap->geSize();

		HDC hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);	
		BitBlt(hDC, pt.x, pt.y, bmSize.width, bmSize.height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
	}
}

void Graphic::drawText(Point pt, LPSTR str)
{
	TextOut(hDC, pt.x, pt.y, str, strlen(str));
}

void Graphic::setFont(Font* pFont)
{
    SelectObject(hDC, pFont->getHandle());
}

void Graphic::setTextColor(Color textColor)
{
    SetTextColor(hDC, textColor);
}


Bitmap* Graphic::copyArea(Bounds bounds)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, bounds.width, bounds.height);
	SelectObject(hMemDC, hBitmap);	
	BitBlt(hMemDC, 0, 0, bounds.width, bounds.height, hDC, bounds.left, bounds.top, SRCCOPY);
	DeleteDC(hMemDC);
	return new Bitmap(hBitmap);
}

void Graphic::drawText(Bounds bounds, LPSTR str, UINT textAlignment)
{
    Rect rc = bounds.toRect();
	DrawText(hDC,  str, strlen(str), &rc, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | textAlignment);
}

//////////////////////////////////

WndGraphic::WndGraphic(HWND hWnd) : Graphic(NULL)
{
    this->hWnd = hWnd;
    hDC = GetDC(hWnd);
}

WndGraphic::~WndGraphic()
{
    ReleaseDC(hWnd, hDC);
}