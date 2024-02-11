#include "StrBuffer.h"

#include <stdarg.h>
#include <stdio.h>

StrBuffer::StrBuffer() 
{
    buffer = NULL;
}


void StrBuffer::format(const char* format, ...)
{
    free();
    static char buff[1024];

    va_list args;
    va_start(args, format);
    int len = vsprintf(buff, format, args);
    if (len > 0) {
        buffer = new char[len];
        strcpy(buffer, buff);
    }
    va_end(args);    
}

void StrBuffer::append(char* text) 
{
    if (buffer == NULL) {
        buffer = new char[strlen(text) + 1];
        strcpy(buffer, text);
    }
    else {
        char* newBuff = new char[strlen(text) + strlen(buffer) + 1];
        strcpy(newBuff, buffer);
        strcat(newBuff, text);
        delete [] buffer;
        buffer = newBuff;
    }
}

StrBuffer::~StrBuffer()
{
    free();
}

void StrBuffer::free() 
{
    if (buffer != NULL) {
        delete [] buffer;
    }
}

char* StrBuffer::allocate(UINT size) 
{
    free();
    buffer = new char[size];
    return buffer;
}

BOOL StrBuffer::equals(char* text)
{
    return (strcmp(buffer, text) == 0);
}

UINT StrBuffer::getLength()
{
    return (buffer != NULL) ? strlen(buffer) : 0;
}

void StrBuffer::set(char* text)
{   
    free();
    append(text);
}