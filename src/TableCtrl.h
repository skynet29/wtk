#ifndef TableCtrl_H
#define TableCtrl_H

#include "Control.h"

class TabCtrl;

class DllExport TableItem {
public:
    void update();
protected:
    virtual char* getColumnData(int numCol) = 0;
    virtual ~TableItem() {}

    friend class TableCtrl;
private:
    TableCtrl* pTableCtrl;    
};

class DllExport TableCtrl : public Control {
public:
	static const UINT K_CENTER;
	static const UINT K_RIGHT;
	static const UINT K_LEFT;

    TableCtrl();
    UINT getItemCount();
    void addColumn(LPSTR strColName, int colWidth, int alignment = K_CENTER);
    UINT getColumnCount();

    void addItem(TableItem *pItem);
    TableItem* getItemAt(UINT idx);
    int getSelIndex();
    void removeItemAt(int index);
    void removeAllItems();
    void updateItem(TableItem* pItem);

protected:
    void create(HWND hParent);
    void onNotify(Event& evt); 
private:
    UINT colIdx;
};

#endif