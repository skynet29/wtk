#ifndef Bounds_H
#define Bounds_H

#include <windows.h>
#include <commctrl.h>


#ifdef DLLMDZ
#define DllExport __declspec( dllexport )
#elif LIBMDZ
#define DllExport
#else
#define DllExport __declspec( dllimport )

#endif

class DllExport Point : public tagPOINT  
{
public:
	Point(int x, int y);
	Point();
};

class DllExport Rect : public tagRECT
{
public:
	Rect(Point p1, Point p2);
	Rect();
	Rect(int left, int top, int right, int bottom);

	void inflate(int dx, int dy);
	BOOL contains(int x, int y);

	int getHeight();
	int getWidth();
};

class DllExport Size {
public:
    int width;
    int height;
    Size();
    Size(int width, int height);
};

class DllExport Bounds  : public Size {
public:
    int left;
    int top;

    Bounds();
    Bounds(int left, int top, int width, int height);
	Rect toRect();
};



#endif