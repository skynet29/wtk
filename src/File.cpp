#include "File.h"
#include "StrVector.h"

File::File()
{
    fd = NULL;
}

File::~File()
{
    close();
}

void File::close()
{
    if (fd != NULL)
    {
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

BOOL File::readTextFile(LPSTR fileName, StrBuffer &text)
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
        return;

    do
    {
        BOOL isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        BOOL isHidden = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;

        if (!isDir && !isHidden)
            vector.add(data.cFileName);

    } while (FindNextFile(hFind, &data));

    FindClose(hFind);
}

void File::findFolder(LPSTR path, StrVector &vector)
{
    WIN32_FIND_DATA data;
    HANDLE hFind;
    StrBuffer strPath;
    strPath.format("%s\\*", path);

    hFind = FindFirstFile(strPath.getBuffer(), &data);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        BOOL isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        BOOL isHidden = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;

        StrBuffer fileName(data.cFileName);

        if (isDir && !isHidden && !fileName.equals(".") && !fileName.equals(".."))
        {
            vector.add(data.cFileName);
        }

    } while (FindNextFile(hFind, &data));

    FindClose(hFind);
}

BOOL File::hasFolder(LPSTR path)
{
    WIN32_FIND_DATA data;
    HANDLE hFind;
    StrBuffer strPath;
    strPath.format("%s\\*", path);
    BOOL ret = FALSE;

    hFind = FindFirstFile(strPath.getBuffer(), &data);

    if (hFind == INVALID_HANDLE_VALUE)
        return FALSE;

    do
    {
        BOOL isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        BOOL isHidden = (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;

        StrBuffer fileName(data.cFileName);

        if (isDir && !isHidden && !fileName.equals(".") && !fileName.equals(".."))
        {
            ret = TRUE;
            break;
        }

    } while (FindNextFile(hFind, &data));

    FindClose(hFind);

    return ret;
}