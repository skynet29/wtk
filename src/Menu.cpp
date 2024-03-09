#include "Menu.h"
#include "Vector.h"

static Vector<Shortcut*> shortcuts;

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
}

void PopupMenu::addItem(UINT id, LPSTR caption, Shortcut* shortcut)
{
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
}

void PopupMenu::addPopupMenu(PopupMenu& menu, LPSTR caption)
{
    AppendMenu(hMenu, MF_POPUP, (UINT) menu.hMenu, caption);
}

void PopupMenu::addSeparator()
{
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
}

void PopupMenu::setItemChecked(UINT id, BOOL isChecked)
{
    CheckMenuItem(hMenu, id, isChecked ? MF_CHECKED : MF_UNCHECKED);
}

void PopupMenu::setItemEnabled(UINT id, BOOL isEnabled)
{
    EnableMenuItem(hMenu, id, isEnabled ? MF_ENABLED : MF_GRAYED);
}

void PopupMenu::setItemIcon(UINT id, Icon* pIcon)
{
    MENUITEMINFO info;
    info.cbSize = sizeof(info); 
    info.fMask = MIIM_FTYPE | MIIM_BITMAP | MIIM_DATA;
    info.hbmpItem = HBMMENU_CALLBACK;
    info.fType = MFT_STRING;   
    info.dwItemData = (DWORD)pIcon;

    SetMenuItemInfo(hMenu, id, FALSE, &info);
}