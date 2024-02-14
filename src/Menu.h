#ifndef Menu_H
#define Menu_H

#include <windows.h>
#include "StrBuffer.h"

class DllExport Shortcut {
public:
    UINT keyChanger;
    char character;
    UINT id;

    Shortcut(char character, UINT keyChanger = 0);

    void getText(StrBuffer&buff);

    static HACCEL Shortcut::createAccelTable();
};

class DllExport PopupMenu {
private:
    HMENU hMenu;
public:
    PopupMenu();
    void addItem(UINT id, LPSTR caption, Shortcut* shortcut = NULL);
    void addSeparator();
    void addPopupMenu(PopupMenu& menu, LPSTR caption);
    void setItemChecked(UINT id, BOOL isChecked);
    void setItemEnabled(UINT id, BOOL isEnabled);
    HMENU getHandle() {return hMenu;}
    friend class Menu;
};

class DllExport Menu {
private:
    HMENU hMenu;
public:
    Menu();
    void addPopupMenu(PopupMenu& menu, LPSTR caption);  

    friend class Frame;  
};



#endif