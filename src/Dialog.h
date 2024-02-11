#ifndef Dialog_H
#define Dialog_H

#include "Container.h"

class DllExport Dialog : public Container {
public:
    Dialog();
    ~Dialog();
    void close(int retCode);
    int run();

protected:
    void handleEvent(TEvent& evt);
    void onCommand(UINT id, UINT code);
};

#endif