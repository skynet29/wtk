#ifndef TFONT_H
#define TFONT_H

#include "types.h"

class DllExport TFont {
private:
    HFONT hFont;
public:
    TFont(LPSTR faceName, UINT size, UINT style = 0);
    ~TFont();
    HFONT getHandle() {return hFont;}

    static void initClass();

    static const UINT ITALIC;
    static const UINT BOLD;    
};

#endif