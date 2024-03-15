

#include "Application.h"
#include "Frame.h"
#include "Button.h"
#include "StrBuffer.h"
#include "ListBox.h"
#include "TextField.h"
#include "Panel.h"
#include "Dialog.h"
#include "ToolBar.h"
#include "Layout.h"
#include "ComboBox.h"
#include "TableCtrl.h"
#include "TreeCtrl.h"
#include "SliderCtrl.h"
#include "TabCtrl.h"
#include "BrowseFolderDialog.h"
#include "Icon.h"
#include "Label.h"
#include "Cursor.h"

#include <stdio.h>


class MyPanel : public Panel
{
public:
    MyPanel() : Panel(WS_BORDER)
    {
        pIcon = Icon::createFromBitmap("bitmaps\\copy.bmp", Color::GREEN);
    }

protected:
    Icon* pIcon;
    void onPaint(Graphic &gr)
    {
        gr.setPen(Color::RED, 1, PS_DASHDOT);
        gr.useHollowBrush();
        gr.drawRect(Bounds(5, 5, 100, 100));
        gr.useHollowPen();
        gr.setBrush(Color::GREEN);
        gr.drawCircle(Point(80, 80), 50);
        gr.drawIcon(Point(100, 30), pIcon);
    }
};

class MyTableItem : public TableItem
{
public:
    StrBuffer name;
    StrBuffer age;

    MyTableItem(LPSTR name, LPSTR age)
    {
        this->name.set(name);
        this->age.set(age);
    }

protected:
    char *getColumnData(int numCol)
    {
        switch (numCol)
        {
        case 0:
            return name.getBuffer();
        case 1:
            return age.getBuffer();
        default:
            return NULL;
        }
    }
};

class MyFrame : public Frame
{
private:
    ListBox *listbox1;
    TextField *text1;
    Button *btn1;
    Button *btn2;
    Font *font;
    MyPanel *panel1;
    Container *cont1;
    Label *label1;
    ComboBox *combo1;
    TableCtrl *table1;
    TreeCtrl *tree1;
    SliderCtrl *slider1;
    TabCtrl *tab1;
    RadioGroup group;

public:
    void initCtrl()
    {
        Layout layout2(cont1, 10, 15);
        layout2.add(listbox1, Size(100, 200));
        layout2.endl();
        layout2.add(combo1, Size(100, 25));
        layout2.endl();
        layout2.add(new RadioButton("Male", group), Size(100, 25));
        layout2.endl();
        layout2.add(new RadioButton("Female", group), Size(100, 25));
        layout2.endl();
        layout2.add(text1, Size(100, 25));
        layout2.endl();
        layout2.add(btn2, Size(100, 25));
        layout2.addBorder("Props");

        Layout layout(this, 10, 40);
        layout.add(label1, Size(100, 25));
        layout.add(btn1, Size(80, 25), 10);
        layout.add(cont1, cont1->getPackSize(), 10);
        layout.add(panel1, Size(200, 200), 10);
        // layout.add(table1, Size(200, 200));
        // layout.add(tree1, Size(200, 200));
        layout.add(tab1, Size(200, 200), 10);
        layout.add(slider1, Size(100, 25), 10);
    }
    MyFrame() : Frame("My first application")
    {

        font = new Font("Courier New", 12, Font::ITALIC);
        setBackColor(Color::getSysColor());

        cont1 = new Container();
        cont1->setBackColor(Color::getSysColor());
        listbox1 = new ListBox(LBS_SORT);
        text1 = new TextField(ES_NUMBER);
        btn1 = new Button("Button1");
        panel1 = new MyPanel();
        label1 = new Label("Hello", Label::K_CENTER);
        combo1 = new ComboBox();
        table1 = new TableCtrl();
        tree1 = new TreeCtrl(TRUE);
        btn2 = new Button("Button 2");
        slider1 = new SliderCtrl();
        tab1 = new TabCtrl();
        label1->setBackColor(Color::WHITE);

        btn1->setOnClick(CBK(MyFrame, btn1_onClick));
        btn2->setOnClick(CBK(MyFrame, btn2_onClick));
        listbox1->setOnDblClick(CBK(MyFrame, listbox1_onDblClick));
        slider1->setOnSelChange(CBK(MyFrame, slider1_onSelChange));
        combo1->setOnSelChange(CBK(MyFrame, combo1_onSelChange));
        tree1->setOnSelChange(CBK(MyFrame, tree1_onSelChange));
        tree1->setOnRightClick(CBK(MyFrame, tree1_onRightClick));


        initCtrl();


    }

protected:
    void slider1_onSelChange(void* from) {
        StrBuffer str;
        str.format("%d", slider1->getValue());
        label1->setText(str.getBuffer());        
    }
        
    void combo1_onSelChange(void* from) {
        StrBuffer str;
        combo1->getSelItem(str);
        showMsg(str.getBuffer());      
    }

    void tree1_onSelChange(void* from) {
        StrBuffer str;
        tree1->getSelNode()->getText(str);
        showMsg(str.getBuffer());   
    }

    void btn2_onClick(void* from) {
        RadioButton* pRadio = group.getSelButton();
        if (pRadio != NULL) {
            StrBuffer str;
            pRadio->getText(str);
            showMsg(str.getBuffer());
        }
    }

    void btn1_onClick(void* from) {
        debugPrint("btn1_onClick\n");
        /*
        Color color = panel1->getBackColor();
        if (color.chooseColor())
        {
            panel1->setBackColor(color);
        }
        */

        BrowseFolderDialog dialog1(getenv("USERPROFILE"));
        if (dialog1.run())
        {
            StrBuffer str;
            dialog1.getPath(str);
            showMsg(str.getBuffer());
        }
    }
    void listbox1_onDblClick(void* from) {
        debugPrint("list1_onDblClick\n");

        StrBuffer str;
        listbox1->getSelItem(str);
        showMsg(str.getBuffer());
    }

    void tree1_onRightClick(void* from)
    {
        //debugPrint("tree1_onRightClick\n");
        Point pt = Cursor::getPos();
        tree1->screenToClient(pt);
        StrBuffer str;
        TreeNode *pNode = tree1->getNodeAt(pt);
        str.format("onRightClick node=%p", pNode);
        showMsg(str.getBuffer());
    }

    BOOL canClose()
    {
        showMsg("Bye");
        return TRUE;
    }

    void onCreate()
    {
        Frame::onCreate();

        listbox1->setFont(font);
        btn1->setFont(font);
        label1->setFont(font);

        listbox1->addItem("Marc");
        listbox1->addItem("Brigitte");
        listbox1->addItem("Quentin");

        combo1->addItem("Marc");
        combo1->addItem("Brigitte");
        combo1->addItem("Quentin");
        combo1->setSelIndex(0);

        tab1->addTab("Tab1", table1);
        tab1->addTab("Tab2", tree1);

        table1->addColumn("Name", 50);
        table1->addColumn("Age", 50);

        table1->addItem(new MyTableItem("Marc", "52"));
        table1->addItem(new MyTableItem("Quentin", "22"));

        Bitmap *pBitmap;

        pBitmap = Bitmap::loadFromFile("bitmaps\\variable.bmp");
        int bmp1 = tree1->addBitmap(pBitmap);
        delete pBitmap;
        pBitmap = Bitmap::loadFromFile("bitmaps\\constant.bmp");
        int bmp2 = tree1->addBitmap(pBitmap);
        delete pBitmap;

        TreeNode *node1 = tree1->addNode("Georges");
        TreeNode *node2 = node1->addNode("Marc", bmp2);
        node1->addNode("Christelle");
        node2->addNode("Quentin");
        tree1->getRootNode()->setExpanded(TRUE);

        slider1->setRange(0, 100);
    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Application app;
    MyFrame frame;

    return app.run(frame);
}
