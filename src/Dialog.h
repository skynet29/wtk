#ifndef Dialog_H
#define Dialog_H

#include "Container.h"

class DllExport Dialog : public Container {
public:
    Dialog(LPSTR strTitle = "Dialog");
    ~Dialog();
    void close(int retCode);
    int run();

protected:
    void handleEvent(Event& evt);
};

#endif