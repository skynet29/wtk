#ifndef CUSTCTRL_H
#define CUSTCTRL_H

#include "Window.h"
#include "Color.h"
#include "Event.h"

class Event;

class DllExport CustCtrl : public Window {
private:
    Color backColor;
    HCURSOR hCursor;
public:   
    CustCtrl();
    void setBackColor(Color backColor);
    Color getBackColor() {return backColor;}
    void startTimer(UINT timerId, UINT delayMs);
    void stopTimer(UINT timerId);
    void setCursor(UINT resId);

protected:
    virtual void handleEvent(Event& evt);
    virtual void processDefault(Event& evt);

    virtual void onCreate() {}
    virtual void onTimer(UINT timerId) {}

private:
    static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static BOOL registerMainWindow();

    friend class Application;
};

#endif