#include "BrowseFolderDialog.h"
#include "Layout.h"

BrowseFolderDialog::BrowseFolderDialog(LPSTR strPath) : Dialog("Select Folder")
{
    this->strPath.set(strPath);

    cont1 = new Container();
    cont1->setBackColor(Color::getSysColor());

    Layout layout2(cont1, 0, 0);
    Button* pBtn;
    pBtn = new Button("OK", TRUE);
    pBtn->setOnClick(CBK(BrowseFolderDialog, btnOk));
    layout2.add(pBtn, Size(60, 25));

    pBtn = new Button("Cancel");
    pBtn->setOnClick(CBK(BrowseFolderDialog, btnCancel));
    layout2.add(pBtn, Size(60, 25), 10);

    Layout layout(this, 10, 10);
    layout.add(tree1 = new FolderTreeCtrl(strPath), Size(500, 400));
    layout.endl();
    layout.addLabel("Folder:", 0, 25);
    layout.addEnd(text1 = new TextField(0), 25, 5);
    layout.endl();
    layout.addRight(cont1, cont1->getPackSize());

    tree1->setOnSelChange(CBK(BrowseFolderDialog, selChange));
}

void BrowseFolderDialog::btnOk(void* from) {
    close(TRUE);
}

void BrowseFolderDialog::btnCancel(void* from) {
    close(FALSE);
}

void BrowseFolderDialog::selChange(void* from)
{
    StrBuffer text(strPath.getBuffer());
    text.append("\\");
    tree1->getSelNode()->getNodePath(text);
    text1->setText(text);
}

void BrowseFolderDialog::getPath(StrBuffer& text)
{
    text.set(strPath.getBuffer());
    text.append("\\");
    TreeNode *pNode = tree1->getSelNode();
    if (pNode != NULL)
        pNode->getNodePath(text);
}


void BrowseFolderDialog::onCreate()
{
    Dialog::onCreate();

    packSize(10, 10);
}