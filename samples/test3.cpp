

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "Panel.h"
#include "Cursor.h"

class Shape
{
public:
    Shape(Bounds bounds, Color color)
    {
        this->bounds = bounds;
        this->color = color;
    }
    Bounds bounds;
    Color color;
};

class MyPanel : public Panel
{
public:
    MyPanel()
    {
        pGraphic = NULL;

        setCursor(Cursor::K_CROSS);
    }

    void setSelColor(Color selColor)
    {
        this->selColor = selColor;
    }

protected:
    Graphic *pGraphic;
    Point p1, p2;
    Vector<Shape *> shapes;
    Color selColor;

    void onCreate()
    {
        Panel::onCreate();

        setVertScrollbar(2000, 100);
    }

    void onPaint(Graphic &gr)
    {
        for (UINT i = 0; i < shapes.getCount(); i++)
        {
            gr.setBrush(shapes[i]->color);
            gr.drawRect(shapes[i]->bounds);
        }
    }

    void onLButtonDown(Point pt)
    {
        p1 = p2 = pt;
        pGraphic = getGraphic();
        pGraphic->setDrawMode(Graphic::K_NOT);
        pGraphic->useHollowBrush();
        pGraphic->drawRect(p1, p2);
    }

    void onMouseMove(Point pt)
    {
        if (pGraphic != NULL)
        {
            pGraphic->drawRect(p1, p2);
            p2 = pt;
            pGraphic->drawRect(p1, p2);
        }
    }

    void onLButtonUp(Point pt)
    {
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

struct ColorEntry
{
    char *label;
    Color color;
    MenuItem *pItem;

    ColorEntry(char *label, Color color)
    {
        this->label = label;
        this->color = color;
        pItem = NULL;
    }
};

static ColorEntry colorMap[] = {
    ColorEntry("Red", Color::RED),
    ColorEntry("Green", Color::GREEN),
    ColorEntry("Blue", Color::BLUE),
};

#define NB_COLOR 3

class MyFrame : public Frame
{
private:
public:
    MyPanel *panel1;
    Menu menu;
    PopupMenu fileMenu;
    PopupMenu colorMenu;
    Color selColor;

    enum
    {
    };

    MyFrame() : Frame("Test 3")
    {
        panel1 = new MyPanel();
        addChild(panel1);
        panel1->setBackColor(Color::CYAN);

        menu.addPopupMenu(colorMenu, "Color");
        colorMenu.setOnInit(CBK(MyFrame, onInitMenu));
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry& e = colorMap[i];
            e.pItem = colorMenu.addItem(e.label);
            e.pItem->setOnClick(CBK(MyFrame, colorItem_onClick));
        }
        setMenu(menu);

        selColor = Color::RED;
        panel1->setSelColor(selColor);
    }

protected:
    void onSize(UINT width, UINT height)
    {
        panel1->setSize(width, height);
    }

    void colorItem_onClick(void* from) 
    {
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry& e = colorMap[i];
            if (e.pItem == from)
            {
                selColor = e.color;
                panel1->setSelColor(selColor);
            }
        }        
    }

    void onInitMenu(void* from)
    {
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry& e = colorMap[i];
            e.pItem->setChecked(e.color == selColor);
        }
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
