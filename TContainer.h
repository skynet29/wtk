#ifndef TCONTAINER_H
#define TCONTAINER_H

#include "TWindow.h"
#include "TVector.h"


class DllExport TContainer : public TCustCtrl {
private:
    TVector<TWindow*> childs;
public:
    ~TContainer();
    
    void addChild(TWindow* child);
    TSize getPackSize();
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onCommand(UINT id, UINT code);
    virtual void onCreate();
    virtual void onSize(UINT width, UINT height) {}

};

#endif