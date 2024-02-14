#ifndef Button_H
#define Button_H

#include "Window.h"

class Bitmap;

class DllExport Label : public Control {
public:
	static const UINT K_LEFT;
	static const UINT K_RIGHT;
	static const UINT K_CENTER;

    Label(LPSTR title, UINT align = K_LEFT);

    void setBackColor(Color backColor);
    void setTextColor(Color textColor);

protected:
    void onDrawItem(Event& evt);
private:
    UINT align;
    Color textColor;
    BOOL isOpaque;
    Color backColor;

};

class DllExport Button : public Control {
public: 
    Button(LPSTR title, UINT id);

    void setBitmap(Bitmap* pBitmap);

protected:
    void Button::onCommand(Event& evt);    
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