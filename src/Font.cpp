#include "Font.h"

static int logPixelSy;

const UINT Font::ITALIC	= 1;
const UINT Font::BOLD    = 2;

static Font defaultGuiFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));

Font::Font(HFONT hFont)
{
    this->hFont = hFont;
}

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

Font* Font::chooseFont(Font* pFont)
{
	LOGFONT lf;

	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof(cf);  
	cf.hwndOwner = GetActiveWindow();
	cf.lpLogFont = &lf; 
	cf.Flags = /*CF_INITTOLOGFONTSTRUCT |*/ CF_SCREENFONTS; 
    if (pFont != NULL) {
        GetObject(pFont->getHandle(), sizeof(lf), &lf);
        cf.Flags |= CF_INITTOLOGFONTSTRUCT;
    }

	if (ChooseFont(&cf))
	{
		HFONT hFont = CreateFontIndirect(&lf);
		return new Font(hFont);
	}
	return NULL;
}

Font* Font::getDefaultGuiFont()
{
	return &defaultGuiFont;
}


Size Font::measureString(LPSTR strText)
{
	HDC hdc = GetDC(NULL);
	SelectObject(hdc, hFont);
	SIZE sz;
	GetTextExtentPoint32(hdc, strText, strlen(strText), &sz);
	ReleaseDC(NULL, hdc);
	return Size(sz.cx, sz.cy);
}

Font::~Font()
{
    if (hFont != NULL) {
        DeleteObject(hFont);
    }
}