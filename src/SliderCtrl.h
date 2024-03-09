#ifndef SLIDERCTRL_H
#define SLIDERCTRL_H

#include "Control.h"

class DllExport SliderCtrl : public Control
{
public:
    SliderCtrl(UINT id);

    void setRange(int min, int max);
    void setValue(int value);
    int getValue();
protected:
    void onHScroll(Event& evt); 

};





#endif