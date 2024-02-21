#ifndef FILE_H
#define FILE_H

#include "types.h"

class DllExport File {
public:
    File();
    ~File();

    BOOL open(LPSTR fileName, LPSTR mode);
    void close();

    BOOL write(LPBYTE pBuffer, UINT bufSize);
    BOOL read(LPBYTE pBuffer, UINT bufSize);

private:
    FILE* fd;
};

#endif