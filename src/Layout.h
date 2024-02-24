#ifndef Layout_H
#define Layout_H

#include "Container.h"

class DllExport Layout {

public:

    Layout(Container* pContainer, int left, int top);

    void add(Window* pCtrl, Size size, int pad = 0);
    void addLabel(LPSTR strText, UINT width, UINT height, int pad = 0);
    void endl(int pad = 10); 
    void addBorder(LPSTR strText = NULL, int pad = 10); 


private:
    int left;
    int top;
    int right;
    int maxLineHeight;
    int curX;
    int curY;
    Container* pContainer;
};

#endif