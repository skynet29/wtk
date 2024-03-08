#include "Cursor.h"

const UINT Cursor::K_ARROW           = 32512;
const UINT Cursor::K_IBEAM           = 32513;
const UINT Cursor::K_WAIT            = 32514;
const UINT Cursor::K_CROSS           = 32515;
const UINT Cursor::K_UPARROW         = 32516;
const UINT Cursor::K_SIZENWSE        = 32642;
const UINT Cursor::K_SIZENESW        = 32643;
const UINT Cursor::K_SIZEWE          = 32644;
const UINT Cursor::K_SIZENS          = 32645;
const UINT Cursor::K_SIZEALL         = 32646;
const UINT Cursor::K_NO              = 32648;
const UINT Cursor::K_HAND            = 32649;
const UINT Cursor::K_HELP            = 32651;

Point Cursor::getPos()
{
    Point pt;
    GetCursorPos(&pt);
    return pt;
}

void Cursor::show() 
{
    ShowCursor(TRUE); 
}

void Cursor::hide() 
{
    ShowCursor(FALSE); 
}

void Cursor::clip(Rect rc)
{
    ClipCursor(&rc);
}

void Cursor::release()
{
    ClipCursor(NULL);
}