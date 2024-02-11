#ifndef Bitmap_H
#define Bitmap_H

#include "types.h"

class DllExport Bitmap {
private:
    HBITMAP hBitmap;
public:
    Bitmap(HBITMAP hBitmap);

    ~Bitmap();
    HBITMAP getHandle() {return hBitmap;}
    Size geSize();

    static Bitmap* loadFromFile(LPSTR fileName);
};


#endif