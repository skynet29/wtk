#ifndef SLIDERCTRL_H
#define SLIDERCTRL_H

#include "Control.h"

class DllExport SliderCtrl : public Control
{
public:
    SliderCtrl();

    void setRange(int min, int max);
    void setValue(int value);
    int getValue();
    void setOnSelChange(Callback* cbk) {onSelChange.set(cbk);}
protected:
    void onHScroll(Event& evt); 
private:
    CbkHolder onSelChange;
};





#endif