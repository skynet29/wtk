#ifndef LABEL_H
#define LABEL_H

#include "Control.h"
#include "Color.h"

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

#endif