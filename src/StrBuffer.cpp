#include "StrBuffer.h"
#include "StrVector.h"

#include <stdarg.h>
#include <stdio.h>

StrBuffer::StrBuffer(char* text)
{
    buffer = NULL;
    if (text != NULL) {
        append(text);
    }
}


char* StrBuffer::format(const char* format, ...)
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
     
    return buffer;  
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
        buffer = NULL;
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

void StrBuffer::split(LPSTR strToken, StrVector &vector)
{
	if (buffer == NULL)
		return;

	LPSTR strDup = strdup(buffer);

	LPSTR strTemp = strtok(strDup, strToken);
	while (strTemp != NULL)
	{
		vector.add(strTemp);
		strTemp = strtok(NULL, strToken);
	}
	::free(strDup);
}

void StrBuffer::toUpperCase()
{
    if (buffer != NULL)
	    strupr(buffer);
}

void StrBuffer::toLowerCase()
{
    if (buffer != NULL)
	    strlwr(buffer);
}

int StrBuffer::toInt()
{
	return atoi(buffer);
}

int StrBuffer::toIntHex()
{
	return strtol(buffer, NULL, 16);
}

void StrBuffer::getSubStr(StrBuffer& text, int startIdx, int length)
{
	if (buffer != NULL)
	{
		int maxLength = getLength() - startIdx;
		
		if (length < 0)
			length = maxLength;
		else
			length = min(length, maxLength);

		if (maxLength > 0)
		{
			char* dest = text.allocate(length + 1);
			memcpy(dest, buffer + startIdx, length);
			dest[length] = 0;
		}
	}
}

int StrBuffer::getIndexOf(char* str, int startIdx)
{
	if (buffer == NULL)
		return -1;

	char* temp = strstr(&buffer[startIdx], str);

	if (temp == NULL)
		return -1;

	return (temp - buffer);
}

char StrBuffer::getCharAt(UINT index)
{
	return (buffer != NULL && index < getLength()) ? buffer[index] : 0;
}