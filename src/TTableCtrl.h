#ifndef TTABLECTRL_H
#define TTABLECTRL_H

#include "TWindow.h"

class DllExport TTableItem {
protected:
    virtual char* getColumnData(int numCol) = 0;
    virtual ~TTableItem() {}

    friend class TTableCtrl;
};

class DllExport TTableCtrl : public TControl {
public:
	static const UINT K_CENTER;
	static const UINT K_RIGHT;
	static const UINT K_LEFT;

    TTableCtrl();
    UINT getItemCount();
    void addColumn(LPSTR strColName, int colWidth, int alignment = K_CENTER);
    void addItem(TTableItem *pItem);
    TTableItem* getItemAt(UINT idx);
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