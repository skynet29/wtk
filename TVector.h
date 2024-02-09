#ifndef TVECTOR_H
#define TVECTOR_H

#include <windows.h>

template <class DllExport T>
class DllExport TVector {
private:
    T* buffer;
    UINT growSize;
    UINT size;
    UINT count;
public:
    TVector(UINT growSize = 10) {
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

    ~TVector() {
        if (buffer != NULL) {
            delete [] buffer;
        }
    }
};

#endif