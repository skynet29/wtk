
#include <windows.h>

#include "Application.h"
#include "Frame.h"
#include "StrBuffer.h"
#include "Panel.h"

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
        setVertScrollbar(HEIGHT, 100);
        setHorzScrollbar(WIDTH, 100);

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
        printf("onLButtonUp pGraphic=%p\n", pGraphic);
        if (pGraphic != NULL)
        {
            pGraphic->setDrawMode(Graphic::K_NORMAL);
            pGraphic->setBrush(selColor);
            pGraphic->drawRect(p1, p2);
            delete pGraphic;
            pGraphic = NULL;

            pt = getPageOrigin();
            printf("org = (%d, %d)\n", pt.x, pt.y);

            HDC hDC = GetDC(NULL);
            HDC hMemDC = CreateCompatibleDC(hDC);
            ReleaseDC(NULL, hDC);

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

enum
{
    IDM_RED = 100,
    IDM_GREEN,
    IDM_BLUE,
    IDM_FILEOPEN,
    IDM_FILESAVE,
    IDM_FILEEXIT
};

struct ColorEntry
{
    char *label;
    UINT id;
    Color color;

    ColorEntry(char *label, UINT id, Color color)
    {
        this->label = label;
        this->id = id;
        this->color = color;
    }
};

static ColorEntry colorMap[] = {
    ColorEntry("Red", IDM_RED, Color::RED),
    ColorEntry("Green", IDM_GREEN, Color::GREEN),
    ColorEntry("Blue", IDM_BLUE, Color::BLUE),
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
        fileMenu.addItem(IDM_FILEOPEN, "Open...");
        fileMenu.addItem(IDM_FILESAVE, "Save");
        fileMenu.addSeparator();
        fileMenu.addItem(IDM_FILEEXIT, "Exit");

        menu.addPopupMenu(colorMenu, "Color");
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry e = colorMap[i];
            colorMenu.addItem(e.id, e.label);
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

    void onCommand(UINT id)
    {
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry e = colorMap[i];
            if (e.id == id)
            {
                selColor = e.color;
                panel1->setSelColor(selColor);
            }
        }

        switch (id)
        {
        case IDM_FILESAVE:
            {
                LPSTR fileName = getSaveFileName("bmp");           
                if (fileName != NULL) {
                    printf("fileName=%s\n", fileName);
                    panel1->pBitmap->saveToFile(fileName);
                }  
            }
         
            break;
        case IDM_FILEEXIT:
            close();
            break;
        case IDM_FILEOPEN:
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
        break;
        }
    }

    void onInitMenu(HMENU hMenu)
    {
        for (UINT i = 0; i < NB_COLOR; i++)
        {
            ColorEntry e = colorMap[i];
            colorMenu.setItemChecked(e.id, e.color == selColor);
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
