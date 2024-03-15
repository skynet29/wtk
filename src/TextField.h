#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "Control.h"

class DllExport TextField : public Control {
public:
    TextField(DWORD style = 0);

    void setMaxChar(UINT maxChar);

};

#endif

