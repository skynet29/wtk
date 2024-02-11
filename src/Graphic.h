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

    Graphic(HDC hdc);
public:
    enum TDrawMode {
        K_NORMAL,
        K_NOT,
        K_XOR
    };

    virtual ~Graphic();

    void drawRect(UINT left, UINT top, UINT width, UINT height);
    void drawEllipse(UINT left, UINT top, UINT width, UINT height);
    void drawCircle(UINT x, UINT y, UINT radius);
    void drawBitmap(int x, int y, Bitmap* pBitmap);
    void drawText(int x, int y, LPSTR str);

    void setDrawMode(TDrawMode mode);
    void seFont(Font* pFont);
    void setPen(Color color, UINT width = 1);
    void setBrush(Color color);
    void useHollowBrush();
    void useHollowPen();

    friend class DllExport Panel;
};

class DllExport WndGraphic : public Graphic {
private:
    HWND hWnd;
    WndGraphic(HWND hWnd);
public:
    ~WndGraphic();

    friend class DllExport Panel;
};

#endif