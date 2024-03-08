#ifndef Bitmap_H
#define Bitmap_H

#include "Color.h"

class Graphic;

class DllExport Bitmap {
private:
    HBITMAP hBitmap;
public:
    Bitmap(UINT width, UINT height, Color backColor = Color::WHITE);

    Bitmap(HBITMAP hBitmap);

    ~Bitmap();
    HBITMAP getHandle() {return hBitmap;}
    Size getSize();
    void replaceColor(Color oldColor, Color newColor);

    Bitmap* createMask(Color clTransparent);
    Bitmap* clone();

    Graphic* getGraphic();
    BOOL saveToFile(LPSTR fileName);

    static Bitmap* loadFromFile(LPSTR fileName);
    static Bitmap* loadFromResource(UINT resId, HMODULE hModule = NULL);

};


#endif