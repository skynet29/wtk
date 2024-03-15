#ifndef TYPES_H
#define TYPES_H

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#define message(ignore)

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>


#ifdef DLLMDZ
#define DllExport __declspec( dllexport )
#elif LIBMDZ
#define DllExport
#else
#define DllExport __declspec( dllimport )

#endif

void DllExport debugPrint(char *fmt, ...);

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
	BOOL contains(Point pt);

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
	Bounds(Point p1, Point p2);
	Rect toRect();
};

template <class T>
class PtrHolder {
public:
	PtrHolder() {
		ptr = NULL;
	}
	~PtrHolder() {
		if (ptr != NULL) delete ptr;
	}
	void set(T* p) {
		if (ptr != NULL) delete ptr;
		ptr = p;
	}
protected:
	T* ptr;
};

class Callback {
public:
	virtual void fire(void* from) = 0;
};

class CbkHolder : public PtrHolder<Callback> {
public:
	void fire(void* from) {
		if (ptr != NULL) ptr->fire(from);
	}
};


template <class T>
class CallbackT : public Callback {
public:
	typedef void (T::*EvProto) (void*);

	void fire(void* from) {
		(pTarget->*func)(from);
	}

	CallbackT(EvProto func, T* pTarget) {
		this->func = func;
		this->pTarget = pTarget;
	}		
private:
	EvProto func;
	T* pTarget;	
};


#define CBK(Class, func) \
	new CallbackT<Class>(func, this)

#endif