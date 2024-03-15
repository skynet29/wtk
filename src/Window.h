#ifndef Window_H
#define Window_H


#include "StrBuffer.h"


class DllExport WndAttr {
public:    
    LPSTR className;
    DWORD style;
    DWORD styleEx;
    Bounds bounds;
    LPSTR title;
    LPVOID param;
    HMENU hMenu;

    WndAttr();

    HWND create(HWND hParent);
    void modifyStyle(DWORD addedStyle, BOOL state);
};




class Container;

class DllExport Window {
protected:
    HWND hWnd;
    WndAttr attr;
    Container* parent;
public:
    Window();
    virtual ~Window() {}

    HWND getHandle() {return hWnd;}
    Container* getParent() {return parent;}

    void setVisible(BOOL isVisible);
    void setParent(Container* parent);
    LRESULT sendMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

    virtual void create(HWND hParent);
    virtual void setSize(int width, int height);

    void getText(StrBuffer& buff);
    void setText(LPSTR text);
    void setLocation(int left, int top);    
    void setBounds(Bounds bounds);
    void setBounds(Rect rc);
    void setFocus();
    void centerToParent();
    Bounds getBounds();
    Size getRealSize();
    Size getClientSize();
    void clientToScreen(Point& pt);
    void screenToClient(Point& pt);
};





#endif