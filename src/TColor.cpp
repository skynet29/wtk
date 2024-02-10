#include "TColor.h"

TColor::TColor()
{
    value = RGB(0, 0, 0);
}

TColor::TColor(UINT r, UINT g, UINT b)
{
    value = RGB(r, g, b);
}

TColor::TColor(COLORREF value)
{
    this->value = value;
}

BOOL TColor::chooseColor()
{
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16]; 

	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = GetActiveWindow();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = value;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
 
	if (ChooseColor(&cc))
	{
		value = cc.rgbResult;
		return TRUE;
	}
	return FALSE;

}

    TColor TColor::BLACK(0, 0, 0);
    TColor TColor::WHITE(255, 255, 255);
    TColor TColor::RED(255, 0, 0);
    TColor TColor::GREEN(0, 255, 0);
    TColor TColor::BLUE(0, 0, 255);
    TColor TColor::CYAN(0, 255, 255);
    TColor TColor::MAGENTA(255, 0, 255);
    TColor TColor::YELLOW(255, 255, 0);
    TColor TColor::LIGHTGREY(192, 192, 192);
    TColor TColor::DARKGREY(128, 128, 128);