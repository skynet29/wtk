#ifndef ToolBar_H
#define ToolBar_H

#include "Control.h"
#include "ImageList.h"

class Bitmap;
class ToolBar;

class DllExport ToolButton {
public:
    ToolButton(ToolBar* parent);
    UINT getId() {return id;}
    void setEnabled(BOOL isEnabled);
    void setChecked(BOOL isChecked);
    void setOnClick(Callback* cbk) {onClick.set(cbk);}
    void setToolTips(LPSTR strToolTips);

    BOOL isChecked();
    BOOL isEnabled();

private:
    UINT id;
    ToolBar* parent;
    StrBuffer toolTips;
    CbkHolder onClick;

    friend class ToolBar;
};

class DllExport ToolBar : public Control {
public:

	static const UINT K_BUTTON;
	static const UINT K_CHECK;
	static const UINT K_CHECKGROUP;

    ToolBar();
    ~ToolBar();

    ToolButton* addButton(Icon* pIcon, BYTE style = K_BUTTON);
    void addSeparator(int size = 5);
    Size resize();

protected:
    ToolButton* getButton(UINT id);
    ImageList* pImageList;

    void create(HWND hParent);    
    void onNotify(Event& evt);
    void onCommand(Event& evt);
};

#endif