#include "Graphic.h"
#include "Icon.h"

Graphic::Graphic(HDC hDC)
{
    this->hDC = hDC;
    this->hPen = NULL;
    this->hBrush = NULL;

    SetBkMode(hDC, TRANSPARENT);
}


void Graphic::setDrawMode(DrawMode::e mode)
{
    SetROP2(hDC, mode);
}


void Graphic::drawRect(Bounds bounds)
{
    Rectangle(hDC, bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
}

void Graphic::drawRect(Point p1, Point p2)
{
    Rectangle(hDC, p1.x, p1.y, p2.x, p2.y);
}

void Graphic::drawLine(Point p1, Point p2)
{
	MoveToEx(hDC, p1.x, p1.y, NULL);
	LineTo(hDC, p2.x, p2.y);
}

void Graphic::drawPolyline(Vector<Point>& pts)
{
    Polyline(hDC, pts.getBuffer(), pts.getCount());
}

void Graphic::drawPolygon(Vector<Point>& pts)
{
    Polygon(hDC, pts.getBuffer(), pts.getCount());
}


void Graphic::drawEllipse(Bounds bounds)
{
    Ellipse(hDC, bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
}

void Graphic::drawCircle(Point center, UINT radius)
{
    drawEllipse(Bounds(center.x - radius, center.y - radius, radius * 2, radius * 2));
}

void Graphic::setBrush(Color color, BrushStyle::e style)
{
    LOGBRUSH lb;
    lb.lbColor = color;
    lb.lbStyle = BS_HATCHED;
    
    switch(style) {
        case BrushStyle::SOLID:
            lb.lbStyle = BS_SOLID;
            break;
        case BrushStyle::HATCHED_CROSS:
            lb.lbHatch = HS_CROSS;
            break;
        case BrushStyle::HATCHED_BDIAGONAL:
            lb.lbHatch = HS_BDIAGONAL;
            break;
        case BrushStyle::HATCHED_DIAGCROSS:
            lb.lbHatch = HS_DIAGCROSS;
            break;
        case BrushStyle::HATCHED_FDIAGONAL:
            lb.lbHatch = HS_FDIAGONAL;
            break;
        case BrushStyle::HATCHED_HORIZONTAL:
            lb.lbHatch = HS_HORIZONTAL;
            break;
        case BrushStyle::HATCHED_VERTICAL:
            lb.lbHatch = HS_VERTICAL;
            break;
        default:
            return;
    }
    hBrush = CreateBrushIndirect(&lb);
    DeleteObject(SelectObject(hDC, hBrush));
}

void Graphic::setPen(Color color, UINT width, PenStyle::e style)
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
		Size bmSize = pBitmap->getSize();

		HDC hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);	
		BitBlt(hDC, pt.x, pt.y, bmSize.width, bmSize.height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
	}
}

void Graphic::drawIcon(Point pt, Icon* pIcon)
{
    DrawIconEx (hDC, pt.x, pt.y, pIcon->getHandle(), 0, 0, 0, NULL, DI_NORMAL); 
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

void Graphic::drawText(Bounds bounds, LPSTR str, TextAlignment::e textAlignment)
{
    Rect rc = bounds.toRect();
	DrawText(hDC,  str, strlen(str), &rc, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | textAlignment);
}


Color Graphic::getPixelColor(Point pt)
{
    return GetPixel(hDC, pt.x, pt.y);
}

void Graphic::setPixelColor(Point pt, Color color)
{
    SetPixel(hDC, pt.x, pt.y, color);
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

////////////////////////:
MemGraphic::MemGraphic(HDC hDC) : Graphic(hDC) {

}

MemGraphic::~MemGraphic() 
{
    DeleteDC(hDC);
}

void Graphic::drawRegion(Region* pRgn)
{
    PaintRgn(hDC, pRgn->getHandle());
}