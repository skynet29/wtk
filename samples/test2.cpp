
#include <windows.h>

#include "TApplication.h"
#include "TFrame.h"
#include "TButton.h"
#include "TStrBuffer.h"
#include "TListBox.h"
#include "TTextField.h"
#include "TPanel.h"
#include "TDialog.h"
#include "TToolBar.h"
#include "TLayout.h"
#include "TComboBox.h"
#include "TTableCtrl.h"

enum {
    ID_BUTTON1 = 100,
    ID_LISTBOX1,
    ID_TEXT1,
    ID_COMBO1,
};

class MyPanel : public TPanel {
public:
    MyPanel() : TPanel(WS_BORDER) {

    }   
protected:
    void onPaint(TGraphic& gr) {
        gr.setPen(TColor::RED, 5);
        gr.useHollowBrush();
        gr.drawRect(5, 5, 100, 100);
        gr.useHollowPen();
        gr.setBrush(TColor::GREEN);
        gr.drawCircle(80, 80, 50);

    }
};

class MyTableItem : public TTableItem {
public:
    TStrBuffer name;
    TStrBuffer age;

    MyTableItem(LPSTR name, LPSTR age) {
        this->name.append(name);
        this->age.append(age);
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

class MyFrame : public TFrame {
private:
    TListBox* listbox1;
    TTextField* text1;
    TButton* btn1;
    TFont* font;
    MyPanel* panel1;
    TContainer* cont1;
    TLabel* label1;
    TComboBox* combo1;
    TTableCtrl* table1;
public:
    void initCtrl() {
        TLayout layout2(cont1, 0, 0, TLayout::K_VERTICAL);
        layout2.add(listbox1, TSize(100, 200));
        layout2.add(combo1, TSize(100, 25));
        layout2.add(text1, TSize(100, 25));

        TLayout layout(this, 10, 40);
        layout.add(label1, TSize(100, 25));
        layout.add(btn1, TSize(80, 25));
        layout.add(cont1, cont1->getPackSize());
        layout.add(panel1, TSize(200, 200));
        layout.add(table1, TSize(200, 200));
    }
    MyFrame() : TFrame("My first application") {

        font = new TFont("Courier New", 12, TFont::ITALIC);

        cont1 = new TContainer();
        listbox1 = new TListBox(ID_LISTBOX1, LBS_SORT);
        text1 = new TTextField(ID_TEXT1, ES_NUMBER);
        btn1 = new TButton("Button", ID_BUTTON1);
        panel1 = new MyPanel();
        label1 = new TLabel("Hello", TColor::RED/*TLabel::K_RIGHT*/);
        combo1 = new TComboBox(ID_COMBO1);
        table1 = new TTableCtrl();

        initCtrl();

        //fileMenu.setItemEnabled(ID_FILEOPEN, FALSE);
        //setBackColor(TColor::CYAN);
        //btn1->setEnabled(FALSE);
    }
protected:

    void onCommand(UINT id, UINT code) {
        TStrBuffer str;
        
        switch(id) {
            case ID_BUTTON1:
                {
                    // TDialog dialog1;
                    // int ret = dialog1.run();
                    // str.format("ret=%d", ret);
                    // showMsg(str.getBuffer());

                    TColor color = panel1->getBackColor();
                    if (color.chooseColor()) {
                        panel1->setBackColor(color);
                    }
                }
                break;

            case ID_LISTBOX1:
                if (code == LBN_SELCHANGE) {
                    listbox1->getItemAt(listbox1->getSelIndex(), str);
                    showMsg(str.getBuffer());
                }            
                break;
            case ID_COMBO1:
                if (code == CBN_SELCHANGE) {
                    combo1->getItemAt(combo1->getSelIndex(), str);
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
        TFrame::onCreate();

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
    TApplication app;
    MyFrame frame;

    return app.run(frame);
}
