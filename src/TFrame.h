#ifndef TFRAME_H
#define TFRAME_H

#include "TContainer.h"
#include "TMenu.h"

class DllExport TFrame : public TContainer {
public:
    TFrame(LPSTR title);

    void setMenu(TMenu& menu);
    void close();
    LPSTR getOpenFileName(LPSTR strFilter = NULL, LPSTR strInitDir = NULL);

protected:
    void handleEvent(TEvent& evt);

    virtual BOOL canClose() {return TRUE;}
};

#endif