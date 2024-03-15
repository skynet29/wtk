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
    void onCreate();

private:
    void selChange(void* from);
    void btnOk(void* from);
    void btnCancel(void* from);

    StrBuffer strPath;
    TreeCtrl *tree1;
    TextField *text1;
    Container *cont1;
};

#endif