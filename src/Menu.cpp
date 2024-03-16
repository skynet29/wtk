#include "Menu.h"
#include "Vector.h"
#include "Application.h"

static Vector<Shortcut*> shortcuts;
static Vector<MenuItem*> items;

Shortcut::Shortcut(char character, UINT keyChanger)
{
    this->character = character;
    this->keyChanger = keyChanger;
    this->id = 0;
}

HACCEL Shortcut::createAccelTable()
{
    UINT nbAccel = shortcuts.getCount();
    LPACCEL lpAccel = new tagACCEL[nbAccel];
    for(UINT idx = 0; idx < nbAccel;  idx++)
    {
        lpAccel[idx].key = shortcuts[idx]->character;
    	lpAccel[idx].cmd = shortcuts[idx]->id;
	    lpAccel[idx].fVirt = shortcuts[idx]->keyChanger | FVIRTKEY;
        delete shortcuts[idx];
    }   
	HACCEL hAccel = CreateAcceleratorTable(lpAccel, nbAccel);	
	delete [] lpAccel;

	return hAccel;    
}

void Shortcut::getText(StrBuffer& buffer)
{
    if (keyChanger & FSHIFT) {
        buffer.append("Shift+");
    }
    if (keyChanger & FCONTROL) {
        buffer.append("Ctrl+");
    }
    if (keyChanger & FALT) {
        buffer.append("Alt+");
    }

	if (character >= VK_F1 && character <= VK_F12)
	{        
        StrBuffer t;
        t.format("F%d", character-VK_F1+1);
		buffer.append(t.getBuffer());
	}   
    else {
        StrBuffer t;
        t.format("%c", toupper(character));
		buffer.append(t.getBuffer());
    } 
}

MenuItem::MenuItem(HMENU hParent, UINT id)
{
    this->hParent = hParent;
    this->id = id;
    this->pIcon = NULL;
    items.add(this);
}

void MenuItem::setChecked(BOOL isChecked)
{
    CheckMenuItem(hParent, id, isChecked ? MF_CHECKED : MF_UNCHECKED);
}

void MenuItem::setEnabled(BOOL isEnabled)
{
    EnableMenuItem(hParent, id, isEnabled ? MF_ENABLED : MF_GRAYED);
}

void MenuItem::setIcon(Icon* pIcon)
{
    this->pIcon = pIcon;
#if (_WIN32_WINNT >= 0x0500)
    MENUITEMINFO info;
    info.cbSize = sizeof(info); 
    info.fMask = MIIM_FTYPE | MIIM_BITMAP;
    info.hbmpItem = HBMMENU_CALLBACK;
    info.fType = MFT_STRING;   

    SetMenuItemInfo(hParent, id, FALSE, &info);
#endif    
}

MenuItem* MenuItem::getFromId(UINT id)
{
    for(UINT i = 0; i< items.getCount(); i++) {
        if (items[i]->getId() == id)
            return items[i];
    }
    return NULL;
}

Menu::Menu()
{
    hMenu = CreateMenu(); 
}

void Menu::addPopupMenu(PopupMenu& menu, LPSTR caption)
{
    AppendMenu(hMenu, MF_POPUP, (UINT) menu.hMenu, caption);
}

PopupMenu::PopupMenu()
{
    hMenu = CreatePopupMenu();
#if (_WIN32_WINNT >= 0x0500)    
    MENUINFO info;
    info.cbSize = sizeof(info);
    info.fMask = MIM_MENUDATA;
    info.dwMenuData = (DWORD)this;
    SetMenuInfo(hMenu, &info);
#endif
}

MenuItem* PopupMenu::addItem(LPSTR caption, Shortcut* shortcut)
{
    static UINT g_Id = 100;

    UINT id = g_Id++;    

    if (shortcut != NULL) {
        StrBuffer t;
        t.format("%s\t", caption);
        shortcut->getText(t);
        AppendMenu(hMenu, MF_STRING, id, t.getBuffer());
        shortcut->id = id;
        shortcuts.add(shortcut);
    }
    else {
        AppendMenu(hMenu, MF_STRING, id, caption);
    }

    return new MenuItem(hMenu, id);
}

void PopupMenu::addPopupMenu(PopupMenu& menu, LPSTR caption)
{
    AppendMenu(hMenu, MF_POPUP, (UINT) menu.hMenu, caption);
}

void PopupMenu::addSeparator()
{
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
}



MenuItem* PopupMenu::track(Point pt)
{
	UINT id = TrackPopupMenu(hMenu, 
		TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY,
		pt.x, pt.y, 0,
		Application::getMainFrame()->getHandle(), NULL);  

    return MenuItem::getFromId(id); 
}