#include "Button.h"
#include "Container.h"
#include "Bitmap.h"

Button::Button(LPSTR title, BOOL isDefault)
{
    attr.title = title;
    attr.style |= BS_PUSHBUTTON;
    attr.className = "BUTTON";
    if (isDefault)
        attr.style |= BS_DEFPUSHBUTTON;
}

void Button::setBitmap(Bitmap *pBitmap)
{
    sendMsg(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)pBitmap->getHandle());
}

void Button::onCommand(Event &evt)
{
    if (evt.getCode() == BN_CLICKED)
        onClick.fire(this);
}

CheckBox::CheckBox(LPSTR title)
{
    attr.title = title;
    attr.style |= BS_AUTOCHECKBOX;
    attr.className = "BUTTON";
}

void CheckBox::onCommand(Event &evt)
{
    if (evt.getCode() == BN_CLICKED)
        onClick.fire(this);
}

void CheckBox::setChecked(BOOL isChecked)
{
    sendMsg(BM_SETCHECK, isChecked);
}

BOOL CheckBox::isChecked()
{
    return (sendMsg(BM_GETCHECK) == BST_CHECKED);
}

RadioButton::RadioButton(LPSTR title, RadioGroup& grp) : CheckBox(title)
{
    attr.style = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON;
    if (grp.add(this))
        attr.style |= WS_GROUP;
}

void RadioButton::create(HWND hParent)
{
    CheckBox::create(hParent);
    if ((attr.style & WS_GROUP) != 0) 
        setChecked(TRUE);
}


RadioButton* RadioGroup::getSelButton()
{
    for(int i = 0; i < btns.getCount(); i++) {
        if (btns[i]->isChecked()) {
            return btns[i];
        }
    }
    return NULL;
}

BOOL RadioGroup::add(RadioButton* pBtn)
{
    BOOL ret = (btns.getCount() == 0);
    btns.add(pBtn);
    return ret;
}


Border::Border(LPSTR title)
{
    attr.title = title;
    attr.style |= BS_GROUPBOX;
    attr.className = "BUTTON";
}