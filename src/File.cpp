#include "File.h"

File::File() 
{
    fd = NULL;
}

File::~ File() 
{
    close();
}

void File::close() {
    if (fd != NULL) {
        fclose(fd);
        fd = NULL;
    }
}

BOOL File::open(LPSTR fileName, LPSTR mode)
{
    fd = fopen(fileName, mode);
    return (fd != NULL);
}

BOOL File::write(LPBYTE pBuffer, UINT bufSize)
{
    return (fwrite(pBuffer, bufSize, 1, fd) != 1);
}

BOOL File::read(LPBYTE pBuffer, UINT bufSize)
{
    return (fread(pBuffer, bufSize, 1, fd) != 1);
}