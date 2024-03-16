#ifndef Menu_H
#define Menu_H

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

class Icon;

class DllExport MenuItem {
public:
    void setChecked(BOOL isChecked);
    void setEnabled(BOOL isEnabled);
    void setIcon(Icon* pIcon);
    void setOnClick(Callback* cbk) {onClick.set(cbk);}
    UINT getId() {return id;}
private:  
    MenuItem(HMENU hParent, UINT id); 
    static MenuItem* getFromId(UINT id);
   
    HMENU hParent;    
    UINT id;
    Icon* pIcon;
    CbkHolder onClick;

    friend class PopupMenu;
    friend class Container;
};

class DllExport PopupMenu {
private:
    HMENU hMenu;
    CbkHolder onInit;
public:
    PopupMenu();
    MenuItem* addItem(LPSTR caption, Shortcut* shortcut = NULL);
    void addSeparator();
    void addPopupMenu(PopupMenu& menu, LPSTR caption);
    MenuItem* track(Point pt);
    void setOnInit(Callback* cbk) {onInit.set(cbk);}

    HMENU getHandle() {return hMenu;}
    friend class Menu;
    friend class Container;

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