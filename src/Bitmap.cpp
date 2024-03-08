#include "Bitmap.h"
#include "Graphic.h"
#include "File.h"

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
	if (hBitmap != NULL)
	{
		DeleteObject(hBitmap);
	}
}

Bitmap *Bitmap::loadFromFile(LPSTR fileName)
{
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

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

Bitmap *Bitmap::loadFromResource(UINT resId, HMODULE hModule)
{
	HBITMAP hBitmap = LoadBitmap((hModule) ? hModule : GetModuleHandle(NULL),
								 MAKEINTRESOURCE(resId));

	return (hBitmap != NULL) ? new Bitmap(hBitmap) : NULL;
}

Bitmap *Bitmap::createMask(Color clTransparent)
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

Bitmap* Bitmap::clone()
{
	HDC hDC = GetDC(NULL);
	HDC hSrcDC = CreateCompatibleDC(hDC);
	Size sz = getSize();

	SelectObject(hSrcDC, hBitmap);

	HDC hDestDC = CreateCompatibleDC(hDC);
	HBITMAP hNewBitmap = CreateCompatibleBitmap(hDC, sz.width, sz.height);
	SelectObject(hDestDC, hNewBitmap);	

	BitBlt(hDestDC, 0, 0, sz.width, sz.height, hSrcDC, 0, 0, SRCCOPY);
	DeleteDC(hSrcDC);
	DeleteDC(hDestDC);
	ReleaseDC(NULL, hDC);
	return new Bitmap(hNewBitmap);

}

void Bitmap::replaceColor(Color clOldColor, Color clNewColor)
{
	Size sz = getSize();

	HDC hDC = GetDC(NULL);
	HDC hMemDC = CreateCompatibleDC(hDC);

	SelectObject(hMemDC, hBitmap);

	for (int y = 0; y < sz.height; y++)
	{
		for (int x = 0; x < sz.width; x++)
		{
			COLORREF color = GetPixel(hMemDC, x, y);
			if (color == clOldColor)
				SetPixel(hMemDC, x, y, clNewColor);
		}
	}

	DeleteDC(hMemDC);
	ReleaseDC(NULL, hDC);
	
}

Graphic *Bitmap::getGraphic()
{
	HDC hDC = GetDC(NULL);
	HDC hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);
	SelectObject(hMemDC, hBitmap);

	return new MemGraphic(hMemDC);
}

BOOL Bitmap::saveToFile(LPSTR fileName)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	File fd;

	if (!fd.open(fileName, "wb"))
	{
		printf("cannot create file %s\n", fileName);
		return FALSE;
	}

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);

	printf("bmPlanes=%d, bmBitsPixel=%d\n", bmp.bmPlanes, bmp.bmBitsPixel);
	printf("bmWidth=%d, bmHeight=%d\n", bmp.bmWidth, bmp.bmHeight);

	int nbBits = bmp.bmPlanes * bmp.bmBitsPixel;

	UINT memSize = sizeof(BITMAPINFOHEADER);

	if (nbBits < 24)
		memSize += sizeof(RGBQUAD) * (1 << nbBits);

	PBITMAPINFOHEADER pHeader = (PBITMAPINFOHEADER)calloc(memSize, 1);

	pHeader->biSize = sizeof(BITMAPINFOHEADER);
	pHeader->biWidth = bmp.bmWidth;
	pHeader->biHeight = bmp.bmHeight;
	pHeader->biPlanes = bmp.bmPlanes;
	pHeader->biBitCount = bmp.bmBitsPixel;
	if (nbBits < 24)
		pHeader->biClrUsed = (1 << nbBits);

	// If the bitmap is not compressed, set the BI_RGB flag.
	pHeader->biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	// The width must be DWORD aligned unless the bitmap is RLE
	// compressed.
	pHeader->biSizeImage = ((pHeader->biWidth * nbBits + 31) & ~31) / 8 * pHeader->biHeight;

	printf("biSizeImage=%d\n", pHeader->biSizeImage);

	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pHeader->biClrImportant = 0;

	LPBYTE lpBits = (LPBYTE)malloc(pHeader->biSizeImage);

	HDC hDC = GetDC(NULL);
	if (!GetDIBits(hDC, hBitmap, 0, pHeader->biHeight, lpBits, (LPBITMAPINFO)pHeader, DIB_RGB_COLORS))
	{
		printf("GetDIBits error\n");
	}
	ReleaseDC(NULL, hDC);

	BITMAPFILEHEADER fileHeader;

	fileHeader.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
	// Compute the size of the entire file.

	// Compute the offset to the array of color indices.
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
						   pHeader->biSize + pHeader->biClrUsed * sizeof(RGBQUAD);

	fileHeader.bfSize = fileHeader.bfOffBits + pHeader->biSizeImage;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;

	// Copy the BITMAPFILEHEADER into the .BMP file.
	fd.write((LPBYTE)&fileHeader, sizeof(BITMAPFILEHEADER));

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	fd.write((LPBYTE) pHeader, sizeof(BITMAPINFOHEADER) + pHeader->biClrUsed * sizeof (RGBQUAD));

	// Copy the array of color indices into the .BMP file.
	fd.write((LPBYTE)lpBits, pHeader->biSizeImage);

	free(pHeader);
	free(lpBits);

	return TRUE;
}