#include "Bitmap.h"
#include "Graphic.h"

Bitmap::Bitmap(HBITMAP hBitmap)
{
    this->hBitmap = hBitmap;
}

Bitmap::Bitmap(UINT width, UINT height, Color backColor)
{
	HDC hDC = GetDC(NULL);
	hBitmap = CreateCompatibleBitmap(hDC, width, height);
	HDC hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);
	SelectObject(hMemDC, hBitmap);
	HBRUSH hBrush = CreateSolidBrush(backColor);
	Rect rc(0, 0, width, height);
	FillRect(hMemDC, &rc, hBrush);
	DeleteObject(hBrush);
	DeleteDC(hMemDC);
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

Size Bitmap::getSize()
{
    Size ret;
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    ret.width = bm.bmWidth;
    ret.height = bm.bmHeight;
	return ret;

}

Bitmap* Bitmap::loadFromResource(UINT resId, HMODULE hModule)
{
	HBITMAP hBitmap = LoadBitmap((hModule) ? hModule : GetModuleHandle(NULL),
		MAKEINTRESOURCE(resId));

	return new Bitmap(hBitmap);
}

Bitmap* Bitmap::createMask(Color clTransparent)
{
	Size sz = getSize();
    
	HBITMAP hMaskBitmap = CreateBitmap(sz.width, sz.height, 1, 1, NULL);
	HDC hDC = GetDC(NULL);

	HDC hSrcDC = CreateCompatibleDC(hDC);
	SelectObject(hSrcDC, hBitmap);
	SetBkColor(hSrcDC, clTransparent);

	HDC hDestDC = CreateCompatibleDC(hDC);
	SelectObject(hDestDC, hMaskBitmap);

	BitBlt(hDestDC, 0, 0, sz.width, sz.height, hSrcDC, 0, 0, SRCCOPY);

	DeleteDC(hSrcDC);
	DeleteDC(hDestDC);
	ReleaseDC(NULL, hDC);

	return new Bitmap(hMaskBitmap);
}

Graphic* Bitmap::getGraphic() 
{
	HDC hDC = GetDC(NULL);
	HDC hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);
	SelectObject(hMemDC, hBitmap);

	return new MemGraphic(hMemDC);
}