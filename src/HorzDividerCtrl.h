#ifndef HORZDIVIDERCTRL_H
#define HORZDIVIDERCTRL_H

#include "Container.h"

class Panel;

class DllExport HorzDividerCtrl : public Container
{
public:
    HorzDividerCtrl(Window* pRightCtrl, Window* pLeftCtrl, UINT leftCtrlWidth = 100);

private:
    void updateWidth(Point p1);
    void clipCursor();
    void onSize(UINT width, UINT height);
    Window* pRightCtrl;
    Window* pLeftCtrl;
    Panel* pSizePanel;
    UINT leftCtrlWidth;

    friend class HorzSizePanel;
};

#endif