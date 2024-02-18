#include "Cursor.h"

Cursor::Cursor(HCURSOR hCursor)
{
    this->hCursor = hCursor;

}

Cursor::~Cursor()
{
    DeleteObject(hCursor);
}


Cursor* Cursor::loadFromResource(int resId)
{
    HCURSOR hCursor = LoadCursor(NULL, MAKEINTRESOURCE(resId));
    return (hCursor != NULL) ? new Cursor(hCursor) : NULL;
}


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
