#ifndef TWINDOW_H
#define TWINDOW_H


#include "types.h"
#include "TStrBuffer.h"
#include "TFont.h"
#include "TColor.h"

class DllExport TWndAttr {
public:    
    LPSTR className;
    DWORD style;
    DWORD styleEx;
    TBounds bounds;
    LPSTR title;
    LPVOID param;
    HMENU hMenu;

    TWndAttr();

    HWND create(HWND hParent);
    void modifyStyle(DWORD addedStyle, BOOL state);
};


class DllExport TContainer;

class DllExport TWindow {
protected:
    HWND hWnd;
    TWndAttr attr;
    TContainer* parent;
public:
    TWindow();
    virtual ~TWindow() {}

    HWND getHandle() {return hWnd;}

    void setVisible(BOOL isVisible);
    LRESULT sendMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

    virtual void create(HWND hParent);
    void getText(TStrBuffer& buff);
    void setText(LPSTR text);
    void setLocation(int left, int top);    
    void setSize(int width, int height);
    void setBounds(TBounds bounds);
    void setBounds(TRect rc);
    void setFocus();
    void centerToParent();
    int showMsg(LPSTR text, LPSTR title = NULL, UINT type = MB_OK); 
    TBounds getBounds();
    TSize getRealSize();

    friend class DllExport TContainer;

};

class DllExport TControl : public TWindow {
public: 
    void setEnabled(BOOL isEnabled);
    void setFont(TFont* font);
protected:
    virtual void onNotify(LPNMHDR lpHeader) {}  
    virtual void onDrawItem(LPDRAWITEMSTRUCT lpDrawItem) {}
    friend class DllExport TContainer;  
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

class DllExport TCustCtrl : public TWindow {
private:
    TColor backColor;
public:   
    TCustCtrl();
    void setBackColor(TColor backColor);
    TColor getBackColor() {return backColor;}
    void startTimer(UINT timerId, UINT delayMs);
    void stopTimer(UINT timerId);
protected:
    virtual void handleEvent(TEvent& evt);

    virtual void onCreate() {}
    virtual void onTimer(UINT timerId) {}

private:
    static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static BOOL registerMainWindow();

    friend class DllExport TApplication;
};

#endif