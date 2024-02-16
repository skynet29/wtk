#include "StrVector.h"

StrVector::~StrVector()
{
    clear();
}

void StrVector::clear()
{
    for(UINT i = 0; i < getCount(); i++) {
        delete [] tab[i];
    }
    tab.clear();
}

void StrVector::add(char* text)
{
    char* p = new char[strlen(text) + 1];
    strcpy(p, text);
    tab.add(p);
}

UINT StrVector::getCount()
{
    return tab.getCount();
}

char* StrVector::operator[](UINT idx)
{
    return (idx < getCount()) ? tab[idx] : NULL;
}