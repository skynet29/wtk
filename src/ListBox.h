#ifndef ListBox_H
#define ListBox_H

#include "Control.h"
#include "StrBuffer.h"

class DllExport ListBox : public Control {
public:
    ListBox(DWORD style = 0);

    void addItem(LPSTR str);
    int getSelIndex();
    void getItemAt(UINT idx, StrBuffer& buff);
    void removeAllItems();
    void removeItemAt(UINT idx);
    void insertItemAt(LPSTR strItem, UINT idx);
    UINT getItemCount();
    BOOL isItemSelected(UINT idx);
    void getSelItem(StrBuffer& text); 
    void setOnSelChange(Callback* cbk) {onSelChange.set(cbk);}
    void setOnDblClick(Callback* cbk) {onDblClick.set(cbk);}
protected:
    void onCommand(Event& evt);    
private:
    CbkHolder onSelChange;
    CbkHolder onDblClick;    
};

#endif