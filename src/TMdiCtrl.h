#ifndef TMDICTRL_H
#define TMDICTRL_H

#include "TContainer.h"

class DllExport TPopupMenu;

class DllExport TMdiChild : public TContainer {
public:
    TMdiChild(LPSTR title = NULL);
};

class DllExport TMdiCtrl : public TControl {
public:
    TMdiCtrl();

    void cascade();
    void tileHorizontally();
    void tileVertically();
    void setWndMenu(TPopupMenu &menu);
    TMdiChild* getActiveChild();
    void createChild(TMdiChild *pChild);
    void processDefault(TEvent& evt); 

private:
    CLIENTCREATESTRUCT ccs;    
};



#endif