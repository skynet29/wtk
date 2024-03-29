#include "CustCtrl.h"
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
    void handleEvent(Event& evt);

    virtual void onPaint(Graphic& gr) {}
    virtual void onMouseMove(Point pt) {}
    virtual void onLButtonDown(Point pt) {}
    virtual void onLButtonUp(Point pt) {}
    virtual void onRButtonDown(Point pt) {}
    virtual void onRButtonUp(Point pt) {}
    virtual void onKeyDown(UINT key) {}
    virtual void onKeyUp(UINT key) {}

private:
    ScrollBar* pVertScrollbar;
    ScrollBar* pHorzScrollbar;

};