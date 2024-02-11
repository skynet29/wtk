#include "Font.h"

static int logPixelSy;

const UINT Font::ITALIC	= 1;
const UINT Font::BOLD    = 2;

Font::Font(LPSTR faceName, UINT size, UINT style)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));

    lf.lfHeight = -MulDiv(size, logPixelSy, 72);
    if ((style & ITALIC) != 0)
        lf.lfItalic = TRUE;
    lf.lfWeight = ((style & BOLD) != 0) ? FW_BOLD : FW_NORMAL;
    strcpy(lf.lfFaceName, faceName);

    hFont = CreateFontIndirect(&lf);
}

void Font::initClass()
{
 	HDC hdc = GetDC(NULL);
	logPixelSy = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);    
}

Font::~Font()
{
    if (hFont != NULL) {
        DeleteObject(hFont);
    }
}