#include "TStrBuffer.h"

#include <stdarg.h>
#include <stdio.h>

TStrBuffer::TStrBuffer() 
{
    buffer = NULL;
}


void TStrBuffer::format(const char* format, ...)
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

void TStrBuffer::append(char* text) 
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

TStrBuffer::~TStrBuffer()
{
    free();
}

void TStrBuffer::free() 
{
    if (buffer != NULL) {
        delete [] buffer;
    }
}

void TStrBuffer::allocate(UINT size) 
{
    free();
    buffer = new char[size];
}