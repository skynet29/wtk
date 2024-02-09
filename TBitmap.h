#ifndef TBITMAP_H
#define TBITMAP_H

#include "types.h"

class DllExport TBitmap {
private:
    HBITMAP hBitmap;
    TBitmap(HBITMAP hBitmap);
public:
    ~TBitmap();
    HBITMAP getHandle() {return hBitmap;}
    TSize getSize();

    static TBitmap* loadFromFile(LPSTR fileName);
};


#endif