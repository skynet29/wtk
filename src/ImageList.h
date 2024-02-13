#ifndef IMAGELIST_H
#define IMAGELIST_H

#include "types.h"

class Bitmap;

class DllExport ImageList {
public:
    ImageList(int cx, int cy, UINT flags = ILC_COLOR24);
    ~ImageList();

    UINT addBitmap(Bitmap* pBitmap);
    HIMAGELIST getHandle() {return hImageList;}


private:
    HIMAGELIST hImageList;
};

#endif