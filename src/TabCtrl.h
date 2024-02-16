#ifndef TABCTRL_H
#define TABCTRL_H

#include "Window.h"
#include "Vector.h"

class DllExport TabCtrl : public Control {
public:
    TabCtrl();
    ~TabCtrl();

    void addTab(LPSTR strCaption, Window* pCtrl);
    Window* getSelTab();
    UINT getSelIndex() {return selIndex;}
    void setSize(int width, int height);

protected:
    void onNotify(Event& evt);
private:
    int selIndex;    
    Vector<Window*> tabs;
};

#endif