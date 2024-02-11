
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

enum {
    ID_BUTTON1 = 100,
    ID_LISTBOX1,
    ID_TEXT1,
    ID_COMBO1,
};

class MyPanel : public Panel {
public:
    MyPanel() : Panel(WS_BORDER) {

    }   
protected:
    void onPaint(Graphic& gr) {
        gr.setPen(Color::RED, 1, PS_DASHDOT);
        gr.useHollowBrush();
        gr.drawRect(5, 5, 100, 100);
        gr.useHollowPen();
        gr.setBrush(Color::GREEN);
        gr.drawCircle(80, 80, 50);

    }
};

class MyTableItem : public TableItem {
public:
    StrBuffer name;
    StrBuffer age;

    MyTableItem(LPSTR name, LPSTR age) {
        this->name.set(name);
        this->age.set(age);
    }
protected:
    char* getColumnData(int numCol) {
        switch(numCol) {
            case 0:
                return name.getBuffer();
            case 1:
                return age.getBuffer();
            default:
                return NULL;
        }
    }
};

class MyFrame : public Frame {
private:
    ListBox* listbox1;
    TextField* text1;
    Button* btn1;
    Font* font;
    MyPanel* panel1;
    Container* cont1;
    Label* label1;
    ComboBox* combo1;
    TableCtrl* table1;
public:
    void initCtrl() {
        Layout layout2(cont1, 0, 0, Layout::K_VERTICAL);
        layout2.add(listbox1, Size(100, 200));
        layout2.add(combo1, Size(100, 25));
        layout2.add(text1, Size(100, 25));

        Layout layout(this, 10, 40);
        layout.add(label1, Size(100, 25));
        layout.add(btn1, Size(80, 25));
        layout.add(cont1, cont1->getPackSize());
        layout.add(panel1, Size(200, 200));
        layout.add(table1, Size(200, 200));
    }
    MyFrame() : Frame("My first application") {

        font = new Font("Courier New", 12, Font::ITALIC);

        cont1 = new Container();
        listbox1 = new ListBox(ID_LISTBOX1, LBS_SORT);
        text1 = new TextField(ID_TEXT1, ES_NUMBER);
        btn1 = new Button("Button", ID_BUTTON1);
        panel1 = new MyPanel();
        label1 = new Label("Hello", Color::RED/*Label::K_RIGHT*/);
        combo1 = new ComboBox(ID_COMBO1);
        table1 = new TableCtrl();

        initCtrl();

        //fileMenu.setItemEnabled(ID_FILEOPEN, FALSE);
        //setBackColor(Color::CYAN);
        //btn1->setEnabled(FALSE);
    }
protected:

    void onCommand(UINT id, UINT code) {
        StrBuffer str;
        
        switch(id) {
            case ID_BUTTON1:
                {
                    // Dialog dialog1;
                    // int ret = dialog1.run();
                    // str.format("ret=%d", ret);
                    // showMsg(str.getBuffer());

                    Color color = panel1->getBackColor();
                    if (color.chooseColor()) {
                        panel1->setBackColor(color);
                    }
                }
                break;

            case ID_LISTBOX1:
                if (code == LBN_SELCHANGE) {
                    listbox1->getSelItem(str);
                    showMsg(str.getBuffer());
                }            
                break;
            case ID_COMBO1:
                if (code == CBN_SELCHANGE) {
                    combo1->getSelItem(str);
                    showMsg(str.getBuffer());
                }            
                break;
        }
    }


    BOOL canClose() {
        showMsg("Bye");
        return TRUE;
    }

    void onCreate() {
        Frame::onCreate();

        //text1->setMaxChar(5);
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

        table1->addColumn("Name", 50);
        table1->addColumn("Age", 50);

        table1->addItem(new MyTableItem("Marc", "52"));
        table1->addItem(new MyTableItem("Quentin", "22"));

    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{ 
    Application app;
    MyFrame frame;

    return app.run(frame);
}
