#include "Button.h"
#include "Container.h"
#include "Bitmap.h"

Button::Button(LPSTR title, UINT id, BOOL isDefault)
{
    attr.title = title;
    attr.style |= BS_PUSHBUTTON;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;
    if (isDefault)
        attr.style |= BS_DEFPUSHBUTTON;
}

void Button::setBitmap(Bitmap *pBitmap)
{
    sendMsg(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)pBitmap->getHandle());
}

void Button::onCommand(Event &evt)
{
    if (HIWORD(evt.wParam) == BN_CLICKED)
        parent->onCommand(LOWORD(evt.wParam));
}

CheckBox::CheckBox(LPSTR title, UINT id)
{
    attr.title = title;
    attr.style |= BS_AUTOCHECKBOX;
    attr.className = "BUTTON";
    attr.hMenu = (HMENU)id;
}

void CheckBox::onCommand(Event &evt)
{
    if (HIWORD(evt.wParam) == BN_CLICKED)
        parent->onCommand(LOWORD(evt.wParam));
}

void CheckBox::setChecked(BOOL isChecked)
{
    sendMsg(BM_SETCHECK, isChecked);
}

BOOL CheckBox::isChecked()
{
    return (sendMsg(BM_GETCHECK) == BST_CHECKED);
}

RadioButton::RadioButton(LPSTR title, UINT id, BOOL isFirst) : CheckBox(title, id)
{
    attr.style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;
    if (isFirst)
        attr.style |= WS_GROUP;
}

void RadioButton::create(HWND hParent)
{
    CheckBox::create(hParent);
    if ((attr.style & WS_GROUP) != 0) 
        setChecked(TRUE);
}

RadioButton* RadioButton::getSelButton(Container* pContainer, UINT* ids, UINT nbId)
{
    for(int i = 0; i < nbId; i++) {
        RadioButton* pRadio = (RadioButton*)pContainer->getControlById(ids[i]);
        if (pRadio->isChecked()) {
            return pRadio;
        }
    }
    return NULL;
}


Border::Border(LPSTR title)
{
    attr.title = title;
    attr.style |= BS_GROUPBOX;
    attr.className = "BUTTON";
}