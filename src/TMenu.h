#ifndef TMENU_H
#define TMENU_H

#include <windows.h>
#include "TStrBuffer.h"

class DllExport TShortcut {
public:
    UINT keyChanger;
    char character;
    UINT id;

    TShortcut(char character, UINT keyChanger = 0);

    void getText(TStrBuffer&buff);

    static HACCEL TShortcut::createAccelTable();
};

class DllExport TPopupMenu {
private:
    HMENU hMenu;
public:
    TPopupMenu();
    void addItem(UINT id, LPSTR caption, TShortcut* shortcut = NULL);
    void addSeparator();
    void addPopupMenu(TPopupMenu& menu, LPSTR caption);
    void setItemChecked(UINT id, BOOL isChecked);
    void setItemEnabled(UINT id, BOOL isEnabled);
    HMENU getHandle() {return hMenu;}
    friend class DllExport TMenu;
};

class DllExport TMenu {
private:
    HMENU hMenu;
public:
    TMenu();
    void addPopupMenu(TPopupMenu& menu, LPSTR caption);  

    friend class DllExport TFrame;  
};



#endif