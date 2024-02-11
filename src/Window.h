#ifndef Window_H
#define Window_H


#include "types.h"
#include "StrBuffer.h"
#include "Font.h"
#include "Color.h"

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


class DllExport Container;

class DllExport Window {
protected:
    HWND hWnd;
    WndAttr attr;
    Container* parent;
public:
    Window();
    virtual ~Window() {}

    HWND getHandle() {return hWnd;}

    void setVisible(BOOL isVisible);
    LRESULT sendMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

    virtual void create(HWND hParent);
    void getText(StrBuffer& buff);
    void setText(LPSTR text);
    void setLocation(int left, int top);    
    void setSize(int width, int height);
    void setBounds(Bounds bounds);
    void setBounds(Rect rc);
    void setFocus();
    void centerToParent();
    int showMsg(LPSTR text, LPSTR title = NULL, UINT type = MB_OK); 
    Bounds getBounds();
    Size getRealSize();

    friend class DllExport Container;

};

class DllExport Control : public Window {
public: 
    void setEnabled(BOOL isEnabled);
    void setFont(Font* font);
protected:
    virtual void onNotify(LPNMHDR lpHeader) {}  
    virtual void onDrawItem(LPDRAWITEMSTRUCT lpDrawItem) {}
    friend class DllExport Container;  
};

class DllExport TEvent {
public:
    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
    LRESULT lResult;

    TEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void processDefault(HWND hMdiClient = NULL);
    void processMdiDefault();
};

class DllExport CustCtrl : public Window {
private:
    Color backColor;
public:   
    CustCtrl();
    void setBackColor(Color backColor);
    Color getBackColor() {return backColor;}
    void startTimer(UINT timerId, UINT delayMs);
    void stopTimer(UINT timerId);
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onCreate() {}
    virtual void onTimer(UINT timerId) {}

private:
    static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static BOOL registerMainWindow();

    friend class DllExport Application;
};

#endif