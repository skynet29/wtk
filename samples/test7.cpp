

#include "Application.h"
#include "Frame.h"
#include "WaveFile.h"
#include "SliderCtrl.h"
#include "Button.h"
#include "Layout.h"

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
        fileMenu.addSeparator();
        fileMenu.addItem(ID_PLAY, "Play");
        fileMenu.addItem(ID_STOP, "Stop");

        setMenu(menuBar);

        Layout layout(this, 10, 10);
        layout.add(pLabelTime = new Label("00:00"), Size(40, 25));
        layout.add(pSlider = new SliderCtrl(ID_SLIDER), Size(400, 25), 10);


        pLabelTime->setBackColor(Color::WHITE);

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
        LONG time = waveFile.getElapsedTime();

        //debugPrint("onTimer %ld\n", time);
        pSlider->setValue(time);
        StrBuffer text;
        LONG min = time / 60;
        LONG sec = time % 60;
        text.format("%02d:%02d", min, sec);
        pLabelTime->setText(text);
    }

    void onAudioEndReached()
    {
        debugPrint("onAudioEndReached\n");
        stopTimer(0);
    }

    WaveFile waveFile;
    SliderCtrl *pSlider;
    Label* pLabelTime;
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

    return app.run(frame);
}
