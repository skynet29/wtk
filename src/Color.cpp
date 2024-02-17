#include "Color.h"

Color::Color()
{
    value = RGB(0, 0, 0);
}

Color::Color(UINT r, UINT g, UINT b)
{
    value = RGB(r, g, b);
}

Color::Color(COLORREF value)
{
    this->value = value;
}

BOOL Color::chooseColor()
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

Color Color::getSysColor(int idx)
{
	return GetSysColor(idx);
}

Color Color::BLACK(0, 0, 0);
Color Color::WHITE(255, 255, 255);
Color Color::RED(255, 0, 0);
Color Color::GREEN(0, 255, 0);
Color Color::BLUE(0, 0, 255);
Color Color::CYAN(0, 255, 255);
Color Color::MAGENTA(255, 0, 255);
Color Color::YELLOW(255, 255, 0);
Color Color::LIGHTGREY(192, 192, 192);
Color Color::DARKGREY(128, 128, 128);