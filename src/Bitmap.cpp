#include "Bitmap.h"

Bitmap::Bitmap(HBITMAP hBitmap)
{
    this->hBitmap = hBitmap;
}
    
Bitmap::~Bitmap()
{
    if (hBitmap != NULL) {
        DeleteObject(hBitmap);
    }
}

Bitmap* Bitmap::loadFromFile(LPSTR fileName)
{
    HBITMAP hBitmap = (HBITMAP) LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    
    return (hBitmap != NULL) ? new Bitmap(hBitmap) : NULL;
}

Size Bitmap::geSize()
{
    Size ret;
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    ret.width = bm.bmWidth;
    ret.height = bm.bmHeight;
	return ret;

}