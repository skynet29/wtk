
#include <windows.h>

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

#include <stdio.h>

enum
{
    ID_BUTTON1 = 100,
    ID_LISTBOX1,
    ID_TEXT1,
    ID_COMBO1,
    ID_TREE1,
    ID_BUTTON2,
    ID_SLIDER1,
    ID_RADIO1,
    ID_RADIO2
};

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

public:
    void initCtrl()
    {
        Layout layout2(cont1, 10, 15);
        layout2.add(listbox1, Size(100, 200));
        layout2.endl();
        layout2.add(combo1, Size(100, 25));
        layout2.endl();
        layout2.add(new RadioButton("Male", ID_RADIO1, TRUE), Size(100, 25));
        layout2.endl();
        layout2.add(new RadioButton("Female", ID_RADIO2), Size(100, 25));
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
        listbox1 = new ListBox(ID_LISTBOX1, LBS_SORT);
        text1 = new TextField(ID_TEXT1, ES_NUMBER);
        btn1 = new Button("Button1", ID_BUTTON1);
        panel1 = new MyPanel();
        label1 = new Label("Hello", Label::K_CENTER);
        combo1 = new ComboBox(ID_COMBO1);
        table1 = new TableCtrl();
        tree1 = new TreeCtrl(ID_TREE1, TRUE);
        btn2 = new Button("Button 2", ID_BUTTON2);
        slider1 = new SliderCtrl(ID_SLIDER1);
        tab1 = new TabCtrl();
        label1->setBackColor(Color::WHITE);

        initCtrl();

        // fileMenu.setItemEnabled(ID_FILEOPEN, FALSE);
        // setBackColor(Color::CYAN);
        // btn1->setEnabled(FALSE);
    }

protected:
    void onRightClick(UINT id, Point pt)
    {
        StrBuffer str;
        TreeNode *pNode = tree1->getNodeAt(pt);
        str.format("onRightClick id=%d node=%p", id, pNode);
        showMsg(str.getBuffer());
    }

    void onDblClick(UINT id)
    {
        StrBuffer str;
        if (id == ID_LISTBOX1)
        {
            listbox1->getSelItem(str);
            showMsg(str.getBuffer());
        }
    }

    void onSelChange(UINT id)
    {

        StrBuffer str;

        switch (id)
        {
        case ID_TREE1:
        {
            tree1->getSelNode()->getText(str);
            showMsg(str.getBuffer());
        }
        break;

        case ID_COMBO1:
            combo1->getSelItem(str);
            showMsg(str.getBuffer());
            break;

        case ID_SLIDER1:
            str.format("%d", slider1->getValue());
            label1->setText(str.getBuffer());
            break;
        }
    }

    void onCommand(UINT id)
    {
        StrBuffer str;

        switch (id)
        {

        case ID_BUTTON1:
        {
            Color color = panel1->getBackColor();
            if (color.chooseColor())
            {
                panel1->setBackColor(color);
            }
        }
        break;

        case ID_BUTTON2:
        {
            // const UINT ids[] = {ID_RADIO1, ID_RADIO2};
            // RadioButton *pRadio = RadioButton::getSelButton(cont1, (UINT *)ids, 2);
            // if (pRadio != NULL)
            // {
            //     pRadio->getText(str);
            //     showMsg(str.getBuffer());
            // }

            BrowseFolderDialog dialog1(getenv("USERPROFILE"));
            if (dialog1.run())
            {
                dialog1.getPath(str);
                showMsg(str.getBuffer());
            }
        }
        break;
        }
    }

    BOOL canClose()
    {
        showMsg("Bye");
        return TRUE;
    }

    void onCreate()
    {
        Frame::onCreate();

        //((RadioButton*)cont1->getControlById(ID_RADIO1))->setChecked(TRUE);

        // text1->setMaxChar(5);
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
