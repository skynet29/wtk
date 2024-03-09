#include "Icon.h"
#include "Bitmap.h"

Icon::Icon(HICON hIcon)
{
    this->hIcon = hIcon;
}

Icon::Icon(Bitmap* pBitmap, Bitmap* pMask)
{
    ICONINFO info;
    info.fIcon = TRUE;
    info.hbmMask = pMask->getHandle();
    info.hbmColor = pBitmap->getHandle();

    hIcon = CreateIconIndirect(&info);
}

Icon::~Icon()
{
    DestroyIcon(hIcon);
}

Icon *Icon::loadFromFile(LPSTR fileName)
{
	HICON hIcon = (HICON)LoadImage(NULL, fileName, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	return (hIcon != NULL) ? new Icon(hIcon) : NULL;
}


Icon *Icon::loadFromResource(UINT resId, HMODULE hModule)
{
	HICON hIcon = LoadIcon((hModule) ? hModule : GetModuleHandle(NULL),
								 MAKEINTRESOURCE(resId));

	return (hIcon != NULL) ? new Icon(hIcon) : NULL;
}

Icon* Icon::createFromBitmap(LPSTR fileName, Color clTransparent)
{
    Bitmap* pBitmap = Bitmap::loadFromFile(fileName);
    if (pBitmap == NULL)
        return NULL;

    Bitmap* pMask = pBitmap->createMask(clTransparent);
    pBitmap->replaceColor(clTransparent, Color::BLACK);
    Icon* pIcon = new Icon(pBitmap, pMask);

    delete pMask;
    delete pBitmap;

    return pIcon;
}