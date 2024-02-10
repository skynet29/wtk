#ifndef TLISTBOX_H
#define TLISTBOX_H

#include "TWindow.h"
#include "TStrBuffer.h"

class DllExport TListBox : public TControl {
public:
    TListBox(UINT id, DWORD style = 0);

    void addItem(LPSTR str);
    int getSelIndex();
    void getItemAt(UINT idx, TStrBuffer& buff);
    void removeAllItems();
    void removeItemAt(UINT idx);
    void insertItemAt(LPSTR strItem, UINT idx);
    UINT getItemCount();
    BOOL isItemSelected(UINT idx);

};
#endif