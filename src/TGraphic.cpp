#include "TGraphic.h"


TGraphic::TGraphic(HDC hDC)
{
    this->hDC = hDC;
    this->hPen = NULL;
    this->hBrush = NULL;
}

void TGraphic::setDrawMode(TDrawMode mode)
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


void TGraphic::drawRect(UINT left, UINT top, UINT width, UINT height)
{
    Rectangle(hDC, left, top, left + width, top + height);
}

void TGraphic::drawEllipse(UINT left, UINT top, UINT width, UINT height)
{
    Ellipse(hDC, left, top, left + width, top + height);
}

void TGraphic::drawCircle(UINT x, UINT y, UINT radius)
{
    drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
}

void TGraphic::setBrush(TColor color)
{
    hBrush = CreateSolidBrush(color);
    DeleteObject(SelectObject(hDC, hBrush));
}

void TGraphic::setPen(TColor color, UINT width)
{
    hPen = CreatePen(PS_SOLID, width, color);
    DeleteObject(SelectObject(hDC, hPen));
}

void TGraphic::useHollowBrush() {
    DeleteObject(SelectObject(hDC, GetStockObject(HOLLOW_BRUSH)));
}

void TGraphic::useHollowPen() {
    DeleteObject(SelectObject(hDC, GetStockObject(NULL_PEN)));
}

TGraphic::~TGraphic() {
    if (hPen != NULL) {
        DeleteObject(hPen);
    }
    if (hBrush != NULL) {
        DeleteObject(hBrush);
    }    
}

void TGraphic::drawBitmap(int x, int y, TBitmap* pBitmap)
{
	HBITMAP hBitmap = pBitmap->getHandle();
	if (hBitmap)
	{
		TSize bmSize = pBitmap->getSize();

		HDC hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);	
		BitBlt(hDC, x, y, bmSize.width, bmSize.height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
	}
}

void TGraphic::drawText(int x, int y, LPSTR str)
{
	TextOut(hDC, x, y, str, strlen(str));
}

void TGraphic::setFont(TFont* pFont)
{
    SelectObject(hDC, pFont->getHandle());
}


//////////////////////////////////

TWndGraphic::TWndGraphic(HWND hWnd) : TGraphic(NULL)
{
    this->hWnd = hWnd;
    hDC = GetDC(hWnd);
}

TWndGraphic::~TWndGraphic()
{
    ReleaseDC(hWnd, hDC);
}