#ifndef TCOLOR_H
#define TCOLOR_H

#include "types.h"

class DllExport TColor {
public:
    COLORREF value;
    TColor();
    TColor(UINT r, UINT g, UINT b);
    TColor(COLORREF value);
    operator COLORREF() {return value;}
    BOOL chooseColor();

    static TColor BLACK;
    static TColor WHITE;
    static TColor RED;
    static TColor GREEN;
    static TColor BLUE;
    static TColor CYAN;
    static TColor MAGENTA;
    static TColor YELLOW;
    static TColor LIGHTGREY;
    static TColor DARKGREY;
};

#endif