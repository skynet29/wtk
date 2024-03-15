

#include "Application.h"
#include "Frame.h"
#include "WaveFile.h"
#include "SliderCtrl.h"
#include "Button.h"
#include "Layout.h"
#include "Label.h"

class MyFrame : public Frame
{
public:

    MyFrame() : Frame("Test 7")
    {
        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem("Open...")->setOnClick(CBK(MyFrame, fileOpen));
        fileMenu.addSeparator();
        pItemPlay = fileMenu.addItem("Play");
        pItemPlay->setOnClick(CBK(MyFrame, play));
        pItemStop = fileMenu.addItem("Stop");
        pItemStop->setOnClick(CBK(MyFrame, stop));
        fileMenu.setOnInit(CBK(MyFrame, onInitMenu));

        setMenu(menuBar);

        Layout layout(this, 10, 10);
        layout.add(pLabelTime = new Label("00:00"), Size(40, 25));
        layout.add(pSlider = new SliderCtrl(), Size(400, 25), 10);

        waveFile.setOnEndReached(CBK(MyFrame, onAudioEndReached));

        pLabelTime->setBackColor(Color::WHITE);

    }

protected:


    void onInitMenu(void* from)
    {
        pItemPlay->setEnabled(!waveFile.isPlaying());
        pItemStop->setEnabled(waveFile.isPlaying());
    }

    void play(void* from) 
    {
        waveFile.play(startTime);
        startTimer(0, 1000);
    }

    void stop(void* from)
    {
        startTime = waveFile.getElapsedTime();
        stopTimer(0);
        waveFile.stop();
    }

    void fileOpen(void* from)
    {
        LPSTR fileName = getOpenFileName("Wave|*.wav");
        if (fileName != NULL)
        {
            if (waveFile.open(fileName))
            {
                pSlider->setRange(0, waveFile.getDuration());
                startTime = 0;
            }
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

    void onAudioEndReached(void* from)
    {
        debugPrint("onAudioEndReached\n");
        stopTimer(0);
    }

    WaveFile waveFile;
    SliderCtrl *pSlider;
    Label* pLabelTime;
    MenuItem* pItemPlay;
    MenuItem* pItemStop;    
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
