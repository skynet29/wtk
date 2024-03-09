#ifndef CONTROL_H
#define CONTROL_H

#include "Window.h"
#include "Font.h"
#include "Event.h"


class DllExport Control : public Window {
public: 
    void setEnabled(BOOL isEnabled);
    void setFont(Font* font);
    UINT getId();
    void create(HWND hParent);
protected:
    virtual void onNotify(Event& evt) {}  
    virtual void onDrawItem(Event& evt) {}
    virtual void onCommand(Event& evt) {}
    virtual void onHScroll(Event& evt) {}
    friend class Container;  
};

#endif