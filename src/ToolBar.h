#ifndef ToolBar_H
#define ToolBar_H

#include "Control.h"

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
    enum {
        K_CUT,
        K_COPY,
        K_PASTE,
        K_UNDO,
        K_REDOW,
        K_DELETE,
        K_FILENEW,
        K_FILEOPEN,
        K_FILESAVE,
        K_PRINTPRE,
        K_PROPERTIES,
        K_HELP,
        K_FIND,
        K_REPLACE,
        K_PRINT
    };

	static const UINT K_BUTTON;
	static const UINT K_CHECK;
	static const UINT K_CHECKGROUP;

    ToolBar();

    ToolButton* addButton(Bitmap* pBitmap, BYTE style = K_BUTTON);
    ToolButton* addStdButton(int idx, BYTE style = K_BUTTON);
    void addSeparator(int size = 5);
    Size resize();

protected:
    ToolButton* getButton(UINT id);

    void create(HWND hParent);    
    void onNotify(Event& evt);
    void onCommand(Event& evt);
};

#endif