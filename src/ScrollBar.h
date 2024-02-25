#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "types.h"

class DllExport ScrollBar {
public:
    ScrollBar(HWND hWnd, UINT range, UINT inc, UINT type);
private:
	void setPos(UINT pos);
	UINT getPos();
	void scroll(UINT  code, UINT trackPos);
	void resize(UINT newSize);
	void Create();

    UINT inc;
    UINT type;
    HWND hWnd;

    friend class Panel;
};

#endif