#include "TBitmap.h"

TBitmap::TBitmap(HBITMAP hBitmap)
{
    this->hBitmap = hBitmap;
}
    
TBitmap::~TBitmap()
{
    if (hBitmap != NULL) {
        DeleteObject(hBitmap);
    }
}

TBitmap* TBitmap::loadFromFile(LPSTR fileName)
{
    HBITMAP hBitmap = (HBITMAP) LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    
    return (hBitmap != NULL) ? new TBitmap(hBitmap) : NULL;
}

TSize TBitmap::getSize()
{
    TSize ret;
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    ret.width = bm.bmWidth;
    ret.height = bm.bmHeight;
	return ret;

}