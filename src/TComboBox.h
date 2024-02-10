#ifndef TCOMBOBOX_H
#define TCOMBOBOX_H

#include "TWindow.h"
#include "TStrBuffer.h"

class DllExport TComboBox : public TControl {
public:
    TComboBox(UINT id);

    void addItem(LPSTR str);
    int getSelIndex();
    void getItemAt(UINT idx, TStrBuffer& buff);
    void removeAllItems();
    void removeItemAt(UINT idx);
    void insertItemAt(LPSTR strItem, UINT idx);
    UINT getItemCount();
    void setSelIndex(int index);

protected:
    void create(HWND hParent);

};
#endif