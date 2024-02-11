#ifndef Button_H
#define Button_H

#include "Window.h"


class DllExport Label : public Control {
public:
	static const UINT K_LEFT;
	static const UINT K_RIGHT;
	static const UINT K_CENTER;

    Label(LPSTR title, Color texColor = Color::BLACK, UINT align = K_LEFT);
protected:
    void onDrawItem(LPDRAWITEMSTRUCT lpDrawItem);
private:
    UINT align;
    Color texColor;    
};

class DllExport Button : public Control {
public: 
    Button(LPSTR title, UINT id);
};

class DllExport CheckBox : public Control {
public: 
    CheckBox(LPSTR title, UINT id);

    void setChecked(BOOL isChecked);
    BOOL isChecked();

};

class DllExport RadioButton : public CheckBox {
public:
    RadioButton(LPSTR title, UINT id);            
};


class DllExport Border: public Control {
public:
    Border(LPSTR title);
};

#endif