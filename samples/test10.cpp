
#include "Application.h"
#include "Label.h"
#include "LayoutManager.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Application app;
    Frame frame("Test 10");

    PackManager *pLayout = new PackManager(&frame);
    Label* pLabel;
    pLayout->add(pLabel = new Label("Label 1"))
        ->fill(PackManagerData::X);
    pLabel->setBackColor(Color::RED);

    pLayout->add(pLabel = new Label("Label 2"))
        ->side(PackManagerData::BOTTOM)
        ->fill(PackManagerData::X);
    pLabel->setBackColor(Color::GREEN);

    pLayout->add(pLabel = new Label("Label 3"))
        ->side(PackManagerData::LEFT)
        ->fill(PackManagerData::Y);
    pLabel->setBackColor(Color::BLUE);

    pLayout->add(pLabel = new Label("Label 4"))
        ->side(PackManagerData::RIGHT)
        ->fill(PackManagerData::Y);
    pLabel->setBackColor(Color::CYAN);

    pLayout->add(pLabel = new Label("Label 4"))
        ->expand(TRUE)
        ->fill(PackManagerData::BOTH);
    pLabel->setBackColor(Color::YELLOW);    

    return app.run(frame);
}