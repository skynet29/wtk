#ifndef TLAYOUT_H
#define TLAYOUT_H

#include "TContainer.h"

class DllExport TLayout {

public:
    enum TLayoutType {
        K_HORIZONTAL, K_VERTICAL
    };
    TLayout(TContainer* pContainer, int left, int top, TLayoutType type = K_HORIZONTAL);

    void add(TWindow* pCtrl, TSize size, int pad = 10);
private:
    int left;
    int top;
    TContainer* pContainer;
    TLayoutType type;    
};

#endif