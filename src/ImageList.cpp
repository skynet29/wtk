#include "ImageList.h"
#include "Bitmap.h"
#include "Icon.h"

ImageList::ImageList(int cx, int cy, UINT flags)
{
    hImageList = ImageList_Create(cx, cy, flags, 1, 1);
}

UINT ImageList::addBitmap(Bitmap* pBitmap)
{
    return ImageList_Add(hImageList, pBitmap->getHandle(), NULL);
}

    
ImageList::~ImageList()
{
    ImageList_Destroy(hImageList);
}


UINT ImageList::addIcon(Icon* pIcon)
{
    return ImageList_AddIcon(hImageList, pIcon->getHandle());
}