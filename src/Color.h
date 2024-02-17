#ifndef Color_H
#define Color_H

#include "types.h"

class DllExport Color {
public:
    COLORREF value;
    Color();
    Color(UINT r, UINT g, UINT b);
    Color(COLORREF value);
    operator COLORREF() {return value;}
    BOOL chooseColor();

    static Color getSysColor(int idx = COLOR_BTNFACE);


    static Color BLACK;
    static Color WHITE;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color CYAN;
    static Color MAGENTA;
    static Color YELLOW;
    static Color LIGHTGREY;
    static Color DARKGREY;
};

#endif