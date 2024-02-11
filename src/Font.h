#ifndef Font_H
#define Font_H

#include "types.h"

class DllExport Font {
private:
    HFONT hFont;
public:
    Font(LPSTR faceName, UINT size, UINT style = 0);
    ~Font();
    HFONT getHandle() {return hFont;}

    static void initClass();

    static const UINT ITALIC;
    static const UINT BOLD;    
};

#endif