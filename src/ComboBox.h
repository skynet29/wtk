#ifndef ComboBox_H
#define ComboBox_H

#include "Control.h"
#include "StrBuffer.h"

class DllExport ComboBox : public Control {
public:
    ComboBox(UINT id);

    void addItem(LPSTR str);
    int getSelIndex();
    void getItemAt(UINT idx, StrBuffer& buff);
    void removeAllItems();
    void removeItemAt(UINT idx);
    void insertItemAt(LPSTR strItem, UINT idx);
    UINT getItemCount();
    void setSelIndex(int index);
    void getSelItem(StrBuffer& text); 

protected:
    void create(HWND hParent);
    void onCommand(Event& evt);

};
#endif