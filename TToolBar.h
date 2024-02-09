#ifndef TTOOLBAR_H
#define TTOOLBAR_H

#include "TWindow.h"

class DllExport TBitmap;

class DllExport TToolBar : public TControl {
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

    TToolBar();

    void addButton(TBitmap* pBitmap, int idCommand, LPSTR strToolTips = NULL, BYTE style = K_BUTTON);
    void addStdButton(int idx, int idCommand, LPSTR strToolTips = NULL, BYTE style = K_BUTTON);
    void addSeparator(int size = 5);
    void setButtonEnabled(int idCommand, BOOL isEnabled);
    void setButtonChecked(int idCommand, BOOL isChecked);
    TSize resize();

    BOOL isButtonChecked(int idCommand);
    BOOL isButtonEnabled(int idCommand);

protected:
    void create(HWND hParent);    
    void onNotify(LPNMHDR lpHeader);
};

#endif