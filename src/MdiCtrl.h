#ifndef MdiCtrl_H
#define MdiCtrl_H

#include "Container.h"
#include "Control.h"

class DllExport PopupMenu;

class DllExport MdiChild : public Container {
public:
    MdiChild(LPSTR title = NULL);
};

class DllExport MdiCtrl : public Control {
public:
    MdiCtrl();

    void cascade();
    void tileHorizontally();
    void tileVertically();
    void setWndMenu(PopupMenu &menu);
    MdiChild* getActiveChild();
    void createChild(MdiChild *pChild);
    void processDefault(Event& evt); 

private:
    CLIENTCREATESTRUCT ccs;    
};



#endif