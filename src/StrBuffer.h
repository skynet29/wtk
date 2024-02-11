#ifndef StrBuffer_H
#define StrBuffer_H

#include "types.h"

class DllExport StrBuffer {
private:
    char* buffer;    
public:
    StrBuffer();
    ~StrBuffer();
    char* getBuffer() {return buffer;}
    char* allocate(UINT size);
    void format(const char* format, ...);
    void free();
    void append(char* text);
    BOOL equals(char* text);
    UINT getLength();
    void set(char* text);

 };

#endif