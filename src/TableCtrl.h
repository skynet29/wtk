#ifndef TableCtrl_H
#define TableCtrl_H

#include "Window.h"

class DllExport TableItem {
protected:
    virtual char* getColumnData(int numCol) = 0;
    virtual ~TableItem() {}

    friend class TableCtrl;
};

class DllExport TableCtrl : public Control {
public:
	static const UINT K_CENTER;
	static const UINT K_RIGHT;
	static const UINT K_LEFT;

    TableCtrl();
    UINT getItemCount();
    void addColumn(LPSTR strColName, int colWidth, int alignment = K_CENTER);
    void addItem(TableItem *pItem);
    TableItem* getItemAt(UINT idx);
    int getSelIndex();
    void removeItemAt(int index);
    void removeAllItems();

protected:
    void create(HWND hParent);
    void onNotify(LPNMHDR lpHeader); 
private:
    UINT colIdx;
};

#endif