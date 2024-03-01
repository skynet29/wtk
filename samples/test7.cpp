

#include "Application.h"
#include "Frame.h"
#include "WaveFile.h"
#include "SliderCtrl.h"

class MyFrame : public Frame
{
public:
    enum
    {
        ID_FILEOPEN = 100,
        ID_SLIDER
    };

    MyFrame() : Frame("Test 7")
    {
        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(ID_FILEOPEN, "Open...");
        setMenu(menuBar);

        addChild(pSlider = new SliderCtrl(ID_SLIDER), Bounds(10, 10, 400, 25));
    }

protected:
    void onCommand(UINT id)
    {
        LPSTR fileName = getOpenFileName("Wave|*.wav");
        if (fileName != NULL)
        {
            if (waveFile.open(this, fileName))
            {
                pSlider->setRange(0, waveFile.getDuration());
                waveFile.play(60);
                startTimer(0, 1000);
            }
        }
    }

    void onTimer(UINT timerId)
    {
        debugPrint("onTimer %ld\n", waveFile.getElapsedTime());
        pSlider->setValue(waveFile.getElapsedTime());
    }

    void onAudioEndReached()
    {
        debugPrint("onAudioEndReached\n");
        stopTimer(0);
    }

    WaveFile waveFile;
    SliderCtrl *pSlider;
    Menu menuBar;
    PopupMenu fileMenu;
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Application app;
    MyFrame frame;

    debugPrint("lpCmdLine=%s\n", lpCmdLine);
    /*
        WAVEOUTCAPS woc;
        unsigned long iNumDevs, i;

        iNumDevs = waveOutGetNumDevs();

        for (i = 0; i < iNumDevs; i++)
        {
            if (!waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS)))
            {
                debugPrint("Device ID #%u: %s, wChannels=%d, dwFormats=%04X, dwSupport=%04X\r\n", i, woc.szPname, woc.wChannels, woc.dwFormats, woc.dwSupport);
            }
        }
        */

    return app.run(frame);
}
