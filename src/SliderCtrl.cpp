#include "SliderCtrl.h"
#include "Container.h"

SliderCtrl::SliderCtrl()
{
	attr.className = TRACKBAR_CLASS;
	attr.style |= TBS_AUTOTICKS;    
}

void SliderCtrl::setRange(int min, int max)
{
	//lpWnd->SendMsg(TBM_SETRANGE, (WPARAM) TRUE, MAKELONG(iMin, iMax));
	sendMsg(TBM_SETRANGEMIN, FALSE, min);
	sendMsg(TBM_SETRANGEMAX, TRUE, max);

}

void SliderCtrl::setValue(int value)
{
	sendMsg(TBM_SETPOS, (WPARAM) TRUE, value); 
}

int SliderCtrl::getValue()
{
    return sendMsg(TBM_GETPOS);
}

void SliderCtrl::onHScroll(Event& evt) 
{
    switch (LOWORD(evt.wParam))
    {
    case TB_PAGEDOWN:
    case TB_PAGEUP:
    case TB_LINEUP:
    case TB_LINEDOWN:
    case TB_THUMBTRACK:
        onSelChange.fire(this);
        break;
    }    
}
