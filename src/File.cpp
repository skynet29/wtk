#include "File.h"
#include "StrVector.h"

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
    return (fwrite(pBuffer, bufSize, 1, fd) == 1);
}

BOOL File::read(LPBYTE pBuffer, UINT bufSize)
{
    return (fread(pBuffer, bufSize, 1, fd) == 1);
}

ULONG File::getSize()
{
	long cur = ftell(fd);
	fseek(fd, 0, SEEK_END);
	long ret = ftell(fd);
	fseek(fd, cur, SEEK_SET);
	return ret;
}

BOOL File::readTextFile(LPSTR fileName, StrBuffer& text)
{
    File fd;
    if (!fd.open(fileName, "rb")) 
        return FALSE;
    
	long length = fd.getSize();

    LPSTR buffer = text.allocate(length + 1);
	if (!fd.read((LPBYTE)buffer, length))
        return FALSE;

    buffer[length] = 0;

    return TRUE;	
}

void File::findFile(LPSTR strFilter, StrVector &vector)
{
	WIN32_FIND_DATA data;
	HANDLE hFind;

	hFind = FindFirstFile(strFilter, &data);

	if (hFind == INVALID_HANDLE_VALUE)
		return ;

	do
	{
		vector.add(data.cFileName);			
	}
	while (FindNextFile(hFind, &data));

	FindClose(hFind);
}