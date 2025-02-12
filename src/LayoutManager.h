#ifndef LayoutManager_H
#define LayoutManager_H

#include "types.h"
#include "Vector.h"

class Container;
class Window;

class DllExport LayoutManager {
public:
    LayoutManager(Container* pContainer);

    virtual void computeBounds(UINT width, UINT height) = 0;
protected:
    Container* pContainer;
};


class DllExport PackManagerData {
public:
    PackManagerData(Window* pCtrl);
    enum Side {LEFT, TOP, RIGHT, BOTTOM};
    enum Fill {NONE, X, Y, BOTH};
    enum Anchor {NW, N, NE, W, CENTER, E, SW, S, SE};

    PackManagerData* width(UINT value) {_width = value; return this;}
    PackManagerData* height(UINT value) {_height = value; return this;}
    PackManagerData* expand(BOOL value) {_expand = value; return this;}
    PackManagerData* side(Side value) {_side = value; return this;}
    PackManagerData* fill(Fill value) {_fill = value; return this;}
    PackManagerData* anchor(Anchor value) {_anchor = value; return this;}

private:   
    UINT _width;
    UINT _height;
    BOOL _expand;
    Side _side;
    Fill _fill;
    Anchor _anchor;
    Window* pCtrl;

    friend class PackManager;

};

class DllExport PackManager : public LayoutManager {
public:
    PackManager(Container* pContainer);
    PackManagerData* add(Window* pCtrl);

    void computeBounds(UINT width, UINT height);
private:
    Vector<PackManagerData*> data;
    
};

#endif