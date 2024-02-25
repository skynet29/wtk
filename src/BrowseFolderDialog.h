#ifndef BROWSEFOLDERDIALOG_H
#define BROWSEFOLDERDIALOG_H

#include "Dialog.h"
#include "TextField.h"
#include "Button.h"
#include "FolderTreeCtrl.h"

class DllExport BrowseFolderDialog : public Dialog
{
public:
    BrowseFolderDialog(LPSTR strPath);
    
    void getPath(StrBuffer& text);

protected:
    void onSelChange(UINT id);
    void onCreate();

private:
    StrBuffer strPath;
    TreeCtrl *tree1;
    TextField *text1;
    Container *cont1;
};

#endif