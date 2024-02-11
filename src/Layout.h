#ifndef Layout_H
#define Layout_H

#include "Container.h"

class DllExport Layout {

public:
    enum LayoutType {
        K_HORIZONTAL, K_VERTICAL
    };
    Layout(Container* pContainer, int left, int top, LayoutType type = K_HORIZONTAL);

    void add(Window* pCtrl, Size size, int pad = 10);
private:
    int left;
    int top;
    Container* pContainer;
    LayoutType type;    
};

#endif