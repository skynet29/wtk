#ifndef Vector_H
#define Vector_H

#include "types.h"

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

    int find(T item) {
        for(int i = 0; i < count; i++) {
            if (buffer[i] == item) return i;
        }
        return -1;
    }

    BOOL removeAt(UINT idx)
    {
        if (idx >= count) return FALSE;

        void* temp = buffer[idx];

        if ( (idx + 1) != count ) 
        {
            MoveMemory(&buffer[idx], &buffer[idx + 1], (count - idx - 1) * sizeof(T));
        }

        count--;

        return TRUE;
    }    

    void remove(T item) {
        int idx = find(item);
        if (idx > 0)
            removeAt(idx);
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