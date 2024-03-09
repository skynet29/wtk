#ifndef ICON_H
#define ICON_H

#include "Color.h"

class Bitmap;

class DllExport Icon 
{
private:
    HICON hIcon;
public:
    Icon(Bitmap* pBitmap, Bitmap* pMask);

    Icon(HICON hIcon);
    ~Icon();

    HICON getHandle() {return hIcon;}

    static Icon* loadFromFile(LPSTR fileName);
    static Icon* loadFromResource(UINT resId, HMODULE hModule = NULL);
    static Icon* createFromBitmap(LPSTR fileName, Color clTransparent);
};




#endif