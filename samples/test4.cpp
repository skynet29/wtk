

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "Panel.h"
#include "Cursor.h"

#define WIDTH 1920
#define HEIGHT 1080

class MyPanel : public Panel
{
public:
    MyPanel()
    {
        pGraphic = NULL;
        pBitmap = new Bitmap(WIDTH, HEIGHT);
        setBackColor(Color::CYAN);
        setCursor(Cursor::K_CROSS);
    }

    void setSelColor(Color selColor)
    {
        this->selColor = selColor;
    }

protected:
    Graphic *pGraphic;
    Bitmap *pBitmap;
    Point p1, p2;
    Color selColor;

    void onCreate()
    {
        Panel::onCreate();

        setVertScrollbar(HEIGHT, 100);
        setHorzScrollbar(WIDTH, 100);
    }

    void onPaint(Graphic &gr)
    {
        if (pBitmap != NULL)
        {
            gr.drawBitmap(Point(0, 0), pBitmap);
        }
    }

    void onLButtonDown(Point pt)
    {
        p1 = p2 = pt;
        pGraphic = getGraphic();
        pGraphic->setDrawMode(DrawMode::NOT);
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
        printf("onLButtonUp pGraphic=%p\n", pGraphic);
        if (pGraphic != NULL)
        {
            pGraphic->setDrawMode(DrawMode::NORMAL);
            pGraphic->setBrush(selColor);
            pGraphic->drawRect(p1, p2);
            delete pGraphic;
            pGraphic = NULL;

            pt = getPageOrigin();
            printf("org = (%d, %d)\n", pt.x, pt.y);

            Graphic *pBmpGraphic = pBitmap->getGraphic();

            Bounds bds(p1, p2);
            bds.top += pt.y;
            bds.left += pt.x;
            pBmpGraphic->setBrush(selColor);
            pBmpGraphic->drawRect(bds);
            delete pBmpGraphic;
        }
    }

    friend class MyFrame;
};


struct ColorEntry
{
    char *label;
    Color color;
    MenuItem* pItem;

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

        menu.addPopupMenu(fileMenu, "File");
        fileMenu.addItem("Open...")->setOnClick(CBK(MyFrame, fileOpen_onClick));
        fileMenu.addItem("Save")->setOnClick(CBK(MyFrame, fileSave_onClick));
        fileMenu.addSeparator();
        fileMenu.addItem("Exit")->setOnClick(CBK(MyFrame, fileExit_onClick));

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

    void fileSave_onClick(void *from)
    {
        LPSTR fileName = getSaveFileName("bmp");
        if (fileName != NULL)
        {
            printf("fileName=%s\n", fileName);
            panel1->pBitmap->saveToFile(fileName);
        }
    }

    void fileExit_onClick(void* from) 
    {
        close();
    }

    void fileOpen_onClick(void* from) 
    {
        LPSTR fileName = getOpenFileName("Bitmap|*.bmp");
        if (fileName != NULL)
        {
            // printf("fileName=%s\n", fileName);
            Bitmap *pBitmap = Bitmap::loadFromFile(fileName);
            if (pBitmap != NULL)
            {
                Graphic *pGraphic = panel1->pBitmap->getGraphic();
                Point pt = panel1->getPageOrigin();
                pGraphic->drawBitmap(pt, pBitmap);
                delete pGraphic;
                delete pBitmap;
                panel1->redraw();
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
