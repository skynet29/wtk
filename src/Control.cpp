#include "Control.h"

UINT Control::getId()
{
    return (UINT)attr.hMenu;
}

void Control::setEnabled(BOOL isEnabled)
{
    attr.modifyStyle(WS_DISABLED, !isEnabled);
    EnableWindow(hWnd, isEnabled);
}

void Control::setFont(Font* font)
{
    sendMsg(WM_SETFONT, (WPARAM) font->getHandle(), TRUE);
}

void Control::create(HWND hParent) {
    Window::create(hParent);
    setFont(Font::getDefaultGuiFont());
}
