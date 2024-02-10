#include "TWindow.h"
#include "TGraphic.h"

class DllExport TPanel : public TCustCtrl {
public:
    TPanel(DWORD style = 0);

    TGraphic* getGraphic();
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onPaint(TGraphic& gr) {}
    virtual void onMouseMove(int x, int y) {}
    virtual void onLButtonDown(int x, int y) {}
    virtual void onLButtonUp(int x, int y) {}
    virtual void onRButtonDown(int x, int y) {}
    virtual void onRButtonUp(int x, int y) {}
};