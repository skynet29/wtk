#ifndef TBUTTON_H
#define TBUTTON_H

#include "TWindow.h"


class DllExport TLabel : public TControl {
public:
	static const UINT K_LEFT;
	static const UINT K_RIGHT;
	static const UINT K_CENTER;

    TLabel(LPSTR title, TColor textColor = TColor::BLACK, UINT align = K_LEFT);
protected:
    void onDrawItem(LPDRAWITEMSTRUCT lpDrawItem);
private:
    UINT align;
    TColor textColor;    
};

class DllExport TButton : public TControl {
public: 
    TButton(LPSTR title, UINT id);
};

class DllExport TCheckBox : public TControl {
public: 
    TCheckBox(LPSTR title, UINT id);

    void setChecked(BOOL isChecked);
    BOOL isChecked();

};

class DllExport TRadioButton : public TCheckBox {
public:
    TRadioButton(LPSTR title, UINT id);            
};


class DllExport TBorder: public TControl {
public:
    TBorder(LPSTR title);
};

#endif