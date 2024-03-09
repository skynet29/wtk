#ifndef Button_H
#define Button_H

#include "Control.h"
#include "Color.h"

class Bitmap;
class Container;


class DllExport Button : public Control {
public: 
    Button(LPSTR title, UINT id, BOOL isDefault = FALSE);

    void setBitmap(Bitmap* pBitmap);

protected:
    void onCommand(Event& evt);    
};

class DllExport CheckBox : public Control {
public: 
    CheckBox(LPSTR title, UINT id);

    void setChecked(BOOL isChecked);
    BOOL isChecked();
protected:
    void onCommand(Event& evt);    
};

class DllExport RadioButton : public CheckBox {
public:
    RadioButton(LPSTR title, UINT id, BOOL isFirst = FALSE);         

    void create(HWND hParent);   

    static RadioButton* getSelButton(Container* pContainer, UINT* ids, UINT nbId);

};


class DllExport Border: public Control {
public:
    Border(LPSTR title);
};

#endif