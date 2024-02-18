
#include <windows.h>

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "Panel.h"

class Shape {
public:
    Shape(Bounds bounds, Color color) {
        this->bounds = bounds;
        this->color = color;
    }
    Bounds bounds;
    Color color;
};

class MyPanel : public Panel {
public:
    MyPanel()     {
        pGraphic = NULL;
    }

    void setSelColor(Color selColor) {
        this->selColor = selColor;
    }
protected:
    Graphic* pGraphic;
    Point p1, p2;
    Vector<Shape*> shapes;
    Color selColor;

    void onPaint(Graphic& gr) {       
        for(UINT i = 0; i < shapes.getCount(); i++) {
            gr.setBrush(shapes[i]->color);
            gr.drawRect(shapes[i]->bounds);
        }
    }

    void onLButtonDown(Point pt) {
        p1 = p2 = pt;
        pGraphic = getGraphic();
        pGraphic->setDrawMode(Graphic::K_NOT);
        pGraphic->useHollowBrush();
        pGraphic->drawRect(p1, p2);
    }

    void onMouseMove(Point pt) {
        if (pGraphic != NULL) {
            pGraphic->drawRect(p1, p2);
            p2 = pt;
            pGraphic->drawRect(p1, p2);
        }
    }

    void onLButtonUp(Point pt) {
        pGraphic->setDrawMode(Graphic::K_NORMAL);
        pGraphic->setBrush(selColor);
        pGraphic->drawRect(p1, p2);
        pt = getPageOrigin();
        Bounds bds = Bounds(p1, p2);
        bds.left += pt.x;
        bds.top += pt.y;
        shapes.add(new Shape(bds, selColor));
        delete pGraphic;
        pGraphic = NULL;
    }
};

enum {IDM_RED = 100, IDM_GREEN, IDM_BLUE};

struct ColorEntry  {
    char* label;
    UINT id;
    Color color;

    ColorEntry(char* label, UINT id, Color color) {
        this->label = label;
        this->id = id;
        this->color = color;
    }
} ;

static ColorEntry colorMap[] = {
    ColorEntry("Red", IDM_RED, Color::RED),
    ColorEntry("Green", IDM_GREEN, Color::GREEN),
    ColorEntry("Blue", IDM_BLUE, Color::BLUE),
};

#define NB_COLOR 3


class MyFrame : public Frame {
private:
public:
    MyPanel* panel1;
    Menu menu;
    PopupMenu fileMenu;
    PopupMenu colorMenu;
    Color selColor;

    enum {IDM_FILEOPEN = 100, IDM_FILEEXIT};

    MyFrame() : Frame("Test 3") {
        panel1 = new MyPanel();
        addChild(panel1);
        panel1->setBackColor(Color::CYAN);
        panel1->setVertScrollbar(2000, 100);

        menu.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(IDM_FILEOPEN, "Open...");
        fileMenu.addSeparator();
        fileMenu.addItem(IDM_FILEEXIT, "Exit");

        menu.addPopupMenu(colorMenu, "Color");
        for(UINT i = 0; i < NB_COLOR; i++) {
            ColorEntry e = colorMap[i];
            colorMenu.addItem(e.id, e.label);
        }
        setMenu(menu);

        selColor = Color::RED;
        panel1->setSelColor(selColor);
    }
protected:
    void onSize(UINT width, UINT height) {
        panel1->setSize(width, height);
    }

    void onCommand(UINT id) {
        for(UINT i = 0; i < NB_COLOR; i++) {
            ColorEntry e = colorMap[i];
            if (e.id == id) {
                selColor = e.color;
                panel1->setSelColor(selColor);
            }
        }

        switch(id) {
            case IDM_FILEEXIT:
                close();
                break;
            case IDM_FILEOPEN:
                {
                    LPSTR fileName = getOpenFileName("Bitmap|*.bmp");
                    if (fileName != NULL) {
                        printf("fileName=%s\n", fileName);
                        Bitmap* pBitmap = Bitmap::loadFromFile(fileName);
                        if (pBitmap != NULL) {
                            Graphic* pGraphic = panel1->getGraphic();
                            pGraphic->drawBitmap(Point(0, 0), pBitmap);
                            delete pGraphic;
                        }
                    }
                }
                break;
        }


    }

    void onInitMenu(HMENU hMenu) {
        for(UINT i = 0; i < NB_COLOR; i++) {
            ColorEntry e = colorMap[i];
            colorMenu.setItemChecked(e.id, e.color == selColor);
        }        
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
