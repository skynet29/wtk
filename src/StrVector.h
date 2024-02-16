#ifndef STRVECTOR_H
#define STRVECTOR_H

#include "Vector.h"

class DllExport StrVector {
public:
    ~StrVector();

    void add(char* text);
    UINT getCount();
    char* operator[](UINT idx);
    void clear();
private:
    Vector<char*> tab;    
};

#endif