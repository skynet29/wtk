#ifndef VERTDIVIDERCTRL_H
#define VERTDIVIDERCTRL_H

#include "Container.h"

class Panel;

class DllExport VertDividerCtrl : public Container
{
public:
    VertDividerCtrl(Window* pTopCtrl, Window* pBottomCtrl, UINT bottomCtrlHeight = 100);

private:
    void onSize(UINT width, UINT height);
    void updateHeight(Point p1);
    void clipCursor();

    Window* pTopCtrl;
    Window* pBottomCtrl;
    Panel* pSizePanel;
    UINT bottomCtrlHeight;

    friend class VertSizePanel;
};

#endif