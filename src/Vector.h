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

    T* getBuffer() {return buffer;}

    void grow() {
        if (count == size) {
            T* tmpBuff = new T[size + growSize];
            if (buffer != NULL) {
                CopyMemory(tmpBuff, buffer, size * sizeof(T));
                delete [] buffer;
            }
            size += growSize;
            buffer = tmpBuff;
        }
    }

    UINT getCount() {
        return count;
    }

    void insertAt(T item, UINT idx)
    {
        if (idx > count)
            throw "Index out of range";

        if (idx == count)
        {
            add(item);
            return;
        }

        grow();
        MoveMemory(&buffer[idx + 1], &buffer[idx], (count - idx) * sizeof(T));
        buffer[idx] = item;
        count++;
    }    

    void add(T item) {
        grow();
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

    void clear() {
        if (buffer != NULL) {
            delete [] buffer;
            buffer = NULL;
        }
        count = 0;
        size = 0;        
    }

    ~Vector() {
        clear();
    }
};

#endif