#ifndef TAPLLICATION_H
#define TAPLLICATION_H

#include "Frame.h"

class DllExport Application {
public:
    Application();
    ~Application();

    int run(Frame& frame);
    static void getExeName(StrBuffer& str);
    static Size getScreenSize();
    static Frame* getMainFrame();

};

#endif