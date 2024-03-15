#ifndef Button_H
#define Button_H

#include "Control.h"
#include "Color.h"
#include "Vector.h"

class Bitmap;
class Container;


class DllExport Button : public Control {
public: 
    Button(LPSTR title, BOOL isDefault = FALSE);

    void setBitmap(Bitmap* pBitmap);
    void setOnClick(Callback* cbk) {onClick.set(cbk);}

protected:
    void onCommand(Event& evt);    
private:
    CbkHolder onClick;    
};

class DllExport CheckBox : public Control {
public: 
    CheckBox(LPSTR title);

    void setChecked(BOOL isChecked);
    BOOL isChecked();
    void setOnClick(Callback* cbk) {onClick.set(cbk);}

protected:
    void onCommand(Event& evt);  
private:
    CbkHolder onClick;        
};

class RadioGroup;

class DllExport RadioButton : public CheckBox {
public:
    RadioButton(LPSTR title, RadioGroup& grp);         

    void create(HWND hParent);   

};

class DllExport RadioGroup {    
public:
    RadioButton* getSelButton();    
private:
    BOOL add(RadioButton* pBtn);
    Vector<RadioButton*> btns;    

    friend class RadioButton;
};

class DllExport Border: public Control {
public:
    Border(LPSTR title);
};

#endif