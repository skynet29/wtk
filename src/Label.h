#ifndef LABEL_H
#define LABEL_H

#include "Control.h"
#include "Color.h"
#include "Graphic.h"


class DllExport Label : public Control {
public:
	static const UINT K_LEFT;
	static const UINT K_RIGHT;
	static const UINT K_CENTER;

    Label(LPSTR title, TextAlignment::e align = TextAlignment::LEFT);

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

#endif