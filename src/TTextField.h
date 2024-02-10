#ifndef TTEXTFIELD_H
#define TTEXTFIELD_H

#include "TWindow.h"

class DllExport TTextField : public TControl {
public:
    TTextField(UINT id, DWORD style = 0);

    void setMaxChar(UINT maxChar);

};

#endif

