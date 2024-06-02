#ifndef REGION_H
#define REGION_H

#include "types.h"

struct CombineMode {
    enum e {
        AND = RGN_AND,
        DIFF = RGN_AND,
        OR = RGN_OR,
        XOR = RGN_XOR
    };
};

class DllExport Region 
{
private:
    HRGN hRegion;
    Region(HRGN hRgn);

public:

    ~Region();
    void combine(Region* pRgn, CombineMode::e mode);
    Region* clone();
    BOOL contains(Point pt);
    Rect getBoundingRect();
    void move(int dx, int dy);
    HRGN getHandle() {return hRegion;}


    static Region* createRect(Bounds& bounds);
    static Region* createRoundRect(Bounds& bounds, Size roundSize);
    static Region* createEllipse(Bounds& bounds);



};

#endif