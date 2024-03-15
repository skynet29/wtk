#ifndef Container_H
#define Container_H

#include "CustCtrl.h"
#include "Vector.h"

#define WSA_EVENT     (WM_USER + 1)


class Control;

class DllExport Container : public CustCtrl {
private:
    Vector<Window*> childs;
public:
    ~Container();
    
    void addChild(Window* child);
    void addChild(Window* child, Bounds bounds);

    Size getPackSize();
    void packSize(int xPad = 0, int yPad = 0);

protected:
    virtual void handleEvent(Event& evt);

    virtual void onCreate();
    virtual void onSize(UINT width, UINT height) {}
    virtual void onFocus() {}
    virtual void onIncomingConnection(SOCKET srvSock, SOCKET sock) {}
    virtual void onDataReceived(SOCKET sock) {}
    virtual void onConnectionClosed(SOCKET sock) {}
};

#endif