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
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onCommand(UINT id, UINT code);
    virtual void onCreate();
    virtual void onSize(UINT width, UINT height) {}
    virtual void onFocus() {}

};

#endif