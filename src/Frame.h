#ifndef Frame_H
#define Frame_H

#include "Container.h"
#include "Menu.h"

class DllExport Frame : public Container {
public:
    Frame(LPSTR title);

    void setMenu(Menu& menu);
    void close();
    LPSTR getOpenFileName(LPSTR strFilter = NULL, LPSTR strInitDir = NULL);
    int showMsg(LPSTR text, LPSTR title = NULL, UINT type = MB_OK);

protected:
    void handleEvent(Event& evt);

    virtual BOOL canClose() {return TRUE;}
};

#endif