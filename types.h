#ifndef TBOUNDS_H
#define TBOUNDS_H

#include <windows.h>

#ifdef DLLMDZ
#define DllExport __declspec( dllexport )
#elif LIBMDZ
#define DllExport
#else
#define DllExport __declspec( dllimport )

#endif

class DllExport TSize {
public:
    int width;
    int height;
    TSize();
    TSize(int width, int height);
};

class DllExport TBounds  : public TSize {
public:
    int left;
    int top;

    TBounds();
    TBounds(int left, int top, int width, int height);
};

class DllExport TPoint : public tagPOINT  
{
public:
	TPoint(int x, int y);
	TPoint();
};

class DllExport TRect : public tagRECT
{
public:
	TRect(TPoint p1, TPoint p2);
	TRect();
	TRect(int left, int top, int right, int bottom);

	void inflate(int dx, int dy);
	BOOL contains(int x, int y);

	int getHeight();
	int getWidth();
};

#endif