#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include "Thread.h"

class DllExport MessageThread : public Thread {
public:
    void postMsg(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);

protected:
    void onRun();
    virtual void onMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

};

#endif