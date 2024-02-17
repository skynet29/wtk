#ifndef Bitmap_H
#define Bitmap_H

#include "Color.h"

class DllExport Bitmap {
private:
    HBITMAP hBitmap;
public:
    Bitmap(HBITMAP hBitmap);

    ~Bitmap();
    HBITMAP getHandle() {return hBitmap;}
    Size getSize();
    Bitmap* createMask(Color clTransparent);


    static Bitmap* loadFromFile(LPSTR fileName);
    static Bitmap* loadFromResource(UINT resId, HMODULE hModule = NULL);

};


#endif