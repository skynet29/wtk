#include "Region.h"


Region::Region(HRGN hRgn)
{
    this->hRegion = hRgn;
}

Region::~Region()
{
    DeleteObject(hRegion);
}

Region* Region::createRect(Bounds& bounds)
{
    HRGN hRgn = CreateRectRgn(bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
    return new Region(hRgn);
}

Region* Region::createRoundRect(Bounds& bounds, Size roundSize)
 {
    HRGN hRgn = CreateRoundRectRgn(bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height, roundSize.width, roundSize.height);
    return new Region(hRgn);     
 }

Region* Region::createEllipse(Bounds& bounds)
{
    HRGN hRgn = CreateEllipticRgn(bounds.left, bounds.top, bounds.left + bounds.width, bounds.top + bounds.height);
    return new Region(hRgn);
}

void Region::combine(Region* pRgn, CombineMode::e mode)
{
    CombineRgn(hRegion, hRegion, pRgn->hRegion, mode);
}

Region* Region::clone()
{
    HRGN hRgn = CreateRectRgn(0,0,0,0);
    CombineRgn(hRgn, hRegion, NULL, RGN_COPY);
    return new Region(hRgn);
}

BOOL Region::contains(Point pt)
{
    return PtInRegion(hRegion, pt.x, pt.y);
}

Rect Region::getBoundingRect()
{
    Rect rc;
    GetRgnBox(hRegion, &rc);
    return rc;
}

void Region::move(int dx, int dy)
{
    OffsetRgn(hRegion, dx, dy);
}