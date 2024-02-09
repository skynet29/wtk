#ifndef TDIALOG_H
#define TDIALOG_H

#include "TContainer.h"

class DllExport TDialog : public TContainer {
public:
    TDialog();
    ~TDialog();
    void close(int retCode);
    int run();

protected:
    void handleEvent(TEvent& evt);
    void onCommand(UINT id, UINT code);
};

#endif