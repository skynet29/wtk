#ifndef Vector_H
#define Vector_H

#include <windows.h>

template <class T>
class DllExport Vector {
private:
    T* buffer;
    UINT growSize;
    UINT size;
    UINT count;
public:
    Vector(UINT growSize = 10) {
        this->count = 0;
        this->buffer = NULL;
        this->growSize = growSize;
        this->size = 0;
    }

    void grow() {
        T* tmpBuff = new T[size + growSize];
        if (buffer != NULL) {
            memcpy(tmpBuff, buffer, size * sizeof(T));
            delete [] buffer;
        }
        size += growSize;
        buffer = tmpBuff;
    }

    UINT getCount() {
        return count;
    }

    void add(T item) {
        if (count == size) {
            grow();
        }
        buffer[count++] = item;
    }

    T operator[](UINT idx) {
        return buffer[idx];
    }

    ~Vector() {
        if (buffer != NULL) {
            delete [] buffer;
        }
    }
};

#endif