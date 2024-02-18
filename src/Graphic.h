#ifndef Graphic_H
#define Graphic_H

#include "Color.h"
#include "Bitmap.h"
#include "Font.h"

class DllExport Graphic {
protected:
    HDC hDC;
    HPEN hPen;
    HBRUSH hBrush;

public:
    enum DrawMode {
        K_NORMAL,
        K_NOT,
        K_XOR
    };

    Graphic(HDC hdc);

    virtual ~Graphic();

    void drawRect(Bounds bounds);
    void drawRect(Point p1, Point p2);
    void drawEllipse(Bounds);
    void drawCircle(Point center, UINT radius);
    void drawBitmap(Point pt, Bitmap* pBitmap);
    void drawText(Point pt, LPSTR str);
    void drawText(Bounds bounds, LPSTR str, UINT textAlignment = DT_LEFT);

    Bitmap* copyArea(Bounds bounds);
    Color getPixelColor(Point pt);
    void setPixelColor(Point pt, Color color);


    void setDrawMode(DrawMode mode);
    void setFont(Font* pFont);
    void setTextColor(Color textColor);
    void setPen(Color color, UINT width = 1, UINT style = PS_SOLID);
    void setBrush(Color color);
    
    void useHollowBrush();
    void useHollowPen();

    friend class Panel;
};

class DllExport WndGraphic : public Graphic {
private:
    HWND hWnd;
    WndGraphic(HWND hWnd);
public:
    ~WndGraphic();

    friend class Panel;
};

class DllExport MemGraphic : public Graphic {
private:
    MemGraphic(HDC hDC);
public:
    ~MemGraphic();

    friend class Bitmap;
};

#endif