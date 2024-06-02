#ifndef Graphic_H
#define Graphic_H

#include "Color.h"
#include "Bitmap.h"
#include "Font.h"
#include "Vector.h"
#include "Region.h"

class Icon;

struct PenStyle {
    enum e {
        SOLID = PS_SOLID,
        DASH = PS_DASH,
        DOT = PS_DOT,
        DASHDOT = PS_DASHDOT,
        DASHDOTDOT = PS_DASHDOTDOT
    };
};

struct DrawMode {
    enum e {
        NORMAL = R2_COPYPEN,
        NOT = R2_NOT,
        XOR = R2_XORPEN
    };
};

struct BrushStyle {
    enum e {
        SOLID,
        HATCHED_BDIAGONAL,
        HATCHED_CROSS,
        HATCHED_DIAGCROSS,
        HATCHED_FDIAGONAL,
        HATCHED_HORIZONTAL,
        HATCHED_VERTICAL
    };
};

struct TextAlignment {
    enum e {
        LEFT = DT_LEFT,
        RIGHT = DT_RIGHT,
        CENTER = DT_CENTER
    };
};

class DllExport Graphic {
protected:
    HDC hDC;
    HPEN hPen;
    HBRUSH hBrush;

public:


    Graphic(HDC hdc);
    HDC getHandle() {return hDC;}

    virtual ~Graphic();

    void drawRect(Bounds bounds);
    void drawRect(Point p1, Point p2);
    void drawEllipse(Bounds);
    void drawCircle(Point center, UINT radius);
    void drawLine(Point p1, Point p2);
    void drawBitmap(Point pt, Bitmap* pBitmap);
    void drawText(Point pt, LPSTR str);
    void drawText(Bounds bounds, LPSTR str, TextAlignment::e textAlignment = TextAlignment::LEFT);
    void drawPolyline(Vector<Point>& pts);
    void drawPolygon(Vector<Point>& pts);
    void drawIcon(Point pt, Icon* pIcon);
    void drawRegion(Region* pRgn);

    Bitmap* copyArea(Bounds bounds);
    Color getPixelColor(Point pt);
    void setPixelColor(Point pt, Color color);


    void setDrawMode(DrawMode::e mode);
    void setFont(Font* pFont);
    void setTextColor(Color textColor);
    void setPen(Color color, UINT width = 1, PenStyle::e style = PenStyle::SOLID);
    void setBrush(Color color, BrushStyle::e style = BrushStyle::SOLID);
    
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