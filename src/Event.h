#ifndef EVENT_H
#define EVENT_H

#include "types.h"

class DllExport Event {
public:
    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
    LRESULT lResult;

    Event(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void processDefault(HWND hMdiClient = NULL);
    void processMdiDefault();
};

#endif