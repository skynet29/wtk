#ifndef Container_H
#define Container_H

#include "Window.h"
#include "Vector.h"


class DllExport Container : public CustCtrl {
private:
    Vector<Window*> childs;
public:
    ~Container();
    
    void addChild(Window* child);
    void addChild(Window* child, Bounds bounds);

    Size getPackSize();

    virtual void onCommand(UINT id);
    virtual void onRightClick(UINT id, Point pt);
    virtual void onSelChange(UINT id);
    virtual void onDblClick(UINT id);

protected:
    virtual void handleEvent(Event& evt);

    virtual void onCreate();
    virtual void onSize(UINT width, UINT height) {}
    virtual void onFocus() {}

};

#endif