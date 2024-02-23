#ifndef FILE_H
#define FILE_H

#include "StrBuffer.h"

class DllExport File {
public:
    File();
    ~File();

    BOOL open(LPSTR fileName, LPSTR mode);
    void close();

    BOOL write(LPBYTE pBuffer, UINT bufSize);
    BOOL read(LPBYTE pBuffer, UINT bufSize);
    ULONG getSize();

    static BOOL readTextFile(LPSTR fileName, StrBuffer& text);
    static void findFile(LPSTR strFilter, StrVector &vector);
    static void findFolder(LPSTR strPath, StrVector &vector);


private:
    FILE* fd;
};

#endif