#include "TMenu.h"
#include "TVector.h"

static TVector<TShortcut*> shortcuts;

TShortcut::TShortcut(char character, UINT keyChanger)
{
    this->character = character;
    this->keyChanger = keyChanger;
    this->id = 0;
}

HACCEL TShortcut::createAccelTable()
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

void TShortcut::getText(TStrBuffer& buffer)
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
        TStrBuffer t;
        t.format("F%d", character-VK_F1+1);
		buffer.append(t.getBuffer());
	}   
    else {
        TStrBuffer t;
        t.format("%c", toupper(character));
		buffer.append(t.getBuffer());
    } 
}


TMenu::TMenu()
{
    hMenu = CreateMenu();
}

void TMenu::addPopupMenu(TPopupMenu& menu, LPSTR caption)
{
    AppendMenu(hMenu, MF_POPUP, (UINT) menu.hMenu, caption);
}

TPopupMenu::TPopupMenu()
{
    hMenu = CreatePopupMenu();
}

void TPopupMenu::addItem(UINT id, LPSTR caption, TShortcut* shortcut)
{
    if (shortcut != NULL) {
        TStrBuffer t;
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

void TPopupMenu::addPopupMenu(TPopupMenu& menu, LPSTR caption)
{
    AppendMenu(hMenu, MF_POPUP, (UINT) menu.hMenu, caption);
}

void TPopupMenu::addSeparator()
{
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
}

void TPopupMenu::setItemChecked(UINT id, BOOL isChecked)
{
    CheckMenuItem(hMenu, id, isChecked ? MF_CHECKED : MF_UNCHECKED);
}

void TPopupMenu::setItemEnabled(UINT id, BOOL isEnabled)
{
    EnableMenuItem(hMenu, id, isEnabled ? MF_ENABLED : MF_GRAYED);
}

