#ifndef TAPLLICATION_H
#define TAPLLICATION_H

#include "TFrame.h"

class DllExport TApplication {
public:
    TApplication();
    ~TApplication();

    int run(TFrame& frame);
};

#endif