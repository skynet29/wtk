#include "TFont.h"

static int logPixelSy;

const UINT TFont::ITALIC	= 1;
const UINT TFont::BOLD    = 2;

TFont::TFont(LPSTR faceName, UINT size, UINT style)
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

void TFont::initClass()
{
 	HDC hdc = GetDC(NULL);
	logPixelSy = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);    
}

TFont::~TFont()
{
    if (hFont != NULL) {
        DeleteObject(hFont);
    }
}