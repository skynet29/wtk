#ifndef StrBuffer_H
#define StrBuffer_H

#include "types.h"

class StrVector;

class DllExport StrBuffer {
private:
    char* buffer;    
public:
    StrBuffer(char* text = NULL);
    ~StrBuffer();
    char* getBuffer() {return buffer;}
    char* allocate(UINT size);
    void format(const char* format, ...);
    void free();
    void append(char* text);
    BOOL equals(char* text);
    UINT getLength();
    void set(char* text);
    void split(LPSTR strToken, StrVector &vector);
    void toUpperCase();
    void toLowerCase();
    int toInt();
    int toIntHex();
    void getSubStr(StrBuffer& text, int startIdx, int length = -1);
    int getIndexOf(char* str, int startIdx = 0);
    char getCharAt(UINT index);
 };

#endif