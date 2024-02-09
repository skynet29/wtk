#ifndef TGRAPHIC_H
#define TGRAPHIC_H

#include "TColor.h"
#include "TBitmap.h"
#include "TFont.h"

class DllExport TGraphic {
protected:
    HDC hDC;
    HPEN hPen;
    HBRUSH hBrush;

    TGraphic(HDC hdc);
public:
    enum TDrawMode {
        K_NORMAL,
        K_NOT,
        K_XOR
    };

    virtual ~TGraphic();

    void drawRect(UINT left, UINT top, UINT width, UINT height);
    void drawEllipse(UINT left, UINT top, UINT width, UINT height);
    void drawCircle(UINT x, UINT y, UINT radius);
    void drawBitmap(int x, int y, TBitmap* pBitmap);
    void drawText(int x, int y, LPSTR str);

    void setDrawMode(TDrawMode mode);
    void setFont(TFont* pFont);
    void setPen(TColor color, UINT width = 1);
    void setBrush(TColor color);
    void useHollowBrush();
    void useHollowPen();

    friend class DllExport TPanel;
};

class DllExport TWndGraphic : public TGraphic {
private:
    HWND hWnd;
    TWndGraphic(HWND hWnd);
public:
    ~TWndGraphic();

    friend class DllExport TPanel;
};

#endif