#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "types.h"

class DllExport ScrollBar {
public:
    ScrollBar(UINT range, UINT inc, UINT type);
private:
    void create(HWND hWnd);
	void setPos(UINT pos);
	UINT getPos();
	void scroll(UINT  code, UINT trackPos);
	void resize(UINT newSize);
	void Create();

    UINT range;
    UINT inc;
    UINT type;
    HWND hWnd;

    friend class Panel;
};

#endif