#ifndef TextField_H
#define TextField_H

#include "Window.h"

class DllExport TextField : public Control {
public:
    TextField(UINT id, DWORD style = 0);

    void setMaxChar(UINT maxChar);

};

#endif

