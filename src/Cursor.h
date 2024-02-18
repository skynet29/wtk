#ifndef CURSOR_H
#define CURSOR_H

#include "types.h"

class DllExport Cursor {
public:
    ~Cursor();
    HCURSOR getHandle() {return hCursor;}

    static Cursor* loadFromResource(int resId);

	static const UINT K_ARROW;
	static const UINT K_IBEAM;           
	static const UINT K_WAIT;            
	static const UINT K_CROSS;           
	static const UINT K_UPARROW;         
	static const UINT K_SIZENWSE;        
	static const UINT K_SIZENESW;        
	static const UINT K_SIZEWE;          
	static const UINT K_SIZENS;          
	static const UINT K_SIZEALL;         
	static const UINT K_NO;              
	static const UINT K_HAND;            
	static const UINT K_HELP;   
        
private:
    Cursor(HCURSOR hCursor);

    HCURSOR hCursor;
};

#endif