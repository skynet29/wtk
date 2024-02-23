#ifndef Font_H
#define Font_H

#include "types.h"

class DllExport Font {
private:
    HFONT hFont;
public:
    Font(HFONT hFont);
    Font(LPSTR faceName, UINT size, UINT style = 0);
    ~Font();
    HFONT getHandle() {return hFont;}
    Size measureString(LPSTR strText);

    static void initClass();
    static Font* chooseFont(Font* pFont = NULL);
    static Font* getDefaultGuiFont();

    static const UINT ITALIC;
    static const UINT BOLD;    
};

#endif