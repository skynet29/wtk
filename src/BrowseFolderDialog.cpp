#include "BrowseFolderDialog.h"
#include "Layout.h"

#define ID_TREE1 100

BrowseFolderDialog::BrowseFolderDialog(LPSTR strPath) : Dialog("Select Folder")
{
    this->strPath.set(strPath);

    cont1 = new Container();
    cont1->setBackColor(Color::getSysColor());

    Layout layout2(cont1, 0, 0);
    layout2.add(new Button("OK", IDOK), Size(60, 25));
    layout2.add(new Button("Cancel", IDCANCEL), Size(60, 25), 10);

    Layout layout(this, 10, 10);
    layout.add(tree1 = new FolderTreeCtrl(ID_TREE1, strPath), Size(500, 400));
    layout.endl();
    layout.addLabel("Folder:", 0, 25);
    layout.addEnd(text1 = new TextField(0), 25, 5);
    layout.endl();
    layout.addRight(cont1, cont1->getPackSize());
}

void BrowseFolderDialog::onCommand(UINT id)
{
    switch (id)
    {
    case IDOK:
        close(TRUE);
        break;

    case IDCANCEL:
        close(FALSE);
        break;
    }
}

void BrowseFolderDialog::getPath(StrBuffer& text)
{
    text.set(strPath.getBuffer());
    text.append("\\");
    tree1->getSelNode()->getNodePath(text);
}

void BrowseFolderDialog::onSelChange(UINT id)
{
    if (id == ID_TREE1)
    {
        StrBuffer text(strPath.getBuffer());
        text.append("\\");
        tree1->getSelNode()->getNodePath(text);
        text1->setText(text);
    }
}

void BrowseFolderDialog::onCreate()
{
    Dialog::onCreate();

    packSize(10, 10);
}