#include "Window.h"
#include "Graphic.h"

class ScrollBar;

class DllExport Panel : public CustCtrl {
public:
    Panel(DWORD style = 0);
    ~Panel();

    void redraw(BOOL eraseBkGnd = TRUE);
    Graphic* getGraphic();
    Point getPageOrigin();
    void setVertScrollbar(UINT pageHeight, UINT inc);
    void setHorzScrollbar(UINT pageWidth, UINT inc);

protected:
    void create(HWND hParent);
    void handleEvent(Event& evt);

    virtual void onPaint(Graphic& gr) {}
    virtual void onMouseMove(int x, int y) {}
    virtual void onLButtonDown(int x, int y) {}
    virtual void onLButtonUp(int x, int y) {}
    virtual void onRButtonDown(int x, int y) {}
    virtual void onRButtonUp(int x, int y) {}
private:
    ScrollBar* pVertScrollbar;
    ScrollBar* pHorzScrollbar;

};