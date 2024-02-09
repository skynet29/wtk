#ifndef TSTRBUFFER_H
#define TSTRBUFFER_H

#include "types.h"

class DllExport TStrBuffer {
private:
    char* buffer;    
public:
    TStrBuffer();
    ~TStrBuffer();
    char* getBuffer() {return buffer;}
    void allocate(UINT size);
    void format(const char* format, ...);
    void free();
    void append(char* text);
 };

#endif