

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
        ID_PLAY,
        ID_STOP,
        ID_SLIDER
    };

    MyFrame() : Frame("Test 7")
    {
        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(ID_FILEOPEN, "Open...");
        fileMenu.addItem(ID_PLAY, "Play");
        fileMenu.addItem(ID_STOP, "Stop");

        setMenu(menuBar);

        addChild(pSlider = new SliderCtrl(ID_SLIDER), Bounds(10, 10, 400, 25));
    }

protected:
    void onInitMenu(HMENU hMenu)
    {
        fileMenu.setItemEnabled(ID_PLAY, !waveFile.isPlaying());
        fileMenu.setItemEnabled(ID_STOP, waveFile.isPlaying());
    }

    void onCommand(UINT id)
    {
        switch (id)
        {
        case ID_PLAY:
            waveFile.play(startTime);
            startTimer(0, 1000);
            break;

        case ID_STOP:
            startTime = waveFile.getElapsedTime();
            stopTimer(0);
            waveFile.stop();
            break;

        case ID_FILEOPEN:
        {
            LPSTR fileName = getOpenFileName("Wave|*.wav");
            if (fileName != NULL)
            {
                if (waveFile.open(this, fileName))
                {
                    pSlider->setRange(0, waveFile.getDuration());
                    startTime = 0;
                }
            }
        }
        break;

        default:
            break;
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
    LONG startTime;
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
