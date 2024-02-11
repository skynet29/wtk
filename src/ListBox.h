#ifndef ListBox_H
#define ListBox_H

#include "Window.h"
#include "StrBuffer.h"

class DllExport ListBox : public Control {
public:
    ListBox(UINT id, DWORD style = 0);

    void addItem(LPSTR str);
    int getSelIndex();
    void getItemAt(UINT idx, StrBuffer& buff);
    void removeAllItems();
    void removeItemAt(UINT idx);
    void insertItemAt(LPSTR strItem, UINT idx);
    UINT getItemCount();
    BOOL isItemSelected(UINT idx);
    void getSelItem(StrBuffer& text); 

};
#endif