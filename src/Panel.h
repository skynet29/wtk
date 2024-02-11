#include "Window.h"
#include "Graphic.h"

class DllExport Panel : public CustCtrl {
public:
    Panel(DWORD style = 0);

    Graphic* geGraphic();
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onPaint(Graphic& gr) {}
    virtual void onMouseMove(int x, int y) {}
    virtual void onLButtonDown(int x, int y) {}
    virtual void onLButtonUp(int x, int y) {}
    virtual void onRButtonDown(int x, int y) {}
    virtual void onRButtonUp(int x, int y) {}
};