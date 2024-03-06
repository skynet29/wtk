
#include <windows.h>

#include "Application.h"
#include "Frame.h"

#include "AviFile.h"
#include "VideoStream.h"
#include "AudioStream.h"
#include "Panel.h"
#include "SliderCtrl.h"

class MyPanel : public Panel {
public:
    void onPaint(Graphic& gr);
};

class MyFrame : public Frame {
public:
    enum {ID_FILEOPEN = 100, ID_PLAY, ID_STOP, ID_SLIDER};

    MyFrame() : Frame("Test 8") {
        pVideoStream = NULL;
        pAudioStream = NULL;
        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem(ID_FILEOPEN, "Open...");
        fileMenu.addSeparator();
        fileMenu.addItem(ID_PLAY, "Play");
        fileMenu.addItem(ID_STOP, "Stop");
        setMenu(menuBar);
        addChild(panel = new MyPanel());
        addChild(slider = new SliderCtrl(ID_SLIDER));

        panel->setBackColor(Color::GREEN);
    }
protected:
    void onSize(UINT width, UINT height) {
        slider->setBounds(Bounds(0, height - 25, width, 25));
        panel->setSize(width, height -25);
    }

    void onAudioEndReached() {
        debugPrint("onAudioEndReached\n");
        stopTimer(0);
    }


    void onTimer(UINT id) {
        if (pAudioStream != NULL) {
            LONG elapsedTime = pAudioStream->getElapsedTime();
            //debugPrint("elapsedTime=%ld\n", elapsedTime);
            slider->setValue(elapsedTime);
            return;
        }
        currentFrame++;
        if (currentFrame > frameCount) {
            stopTimer(0);
            debugPrint("video finish");
        }
        else {
            slider->setValue(currentFrame);
            panel->redraw(FALSE);
        }
    }

    void play() {
         if (pAudioStream != NULL) {
             pAudioStream->play(startTime);
             startTimer(0, 10);
         }
         else {
            LONG frameRate = pVideoStream->getFrameCount() * 1000 / pVideoStream->getDuration();
            startTimer(0, 1000 / frameRate);
         }
    }

    void stop() {
        if (pAudioStream != NULL) {
            startTime = pAudioStream->getElapsedTime();
            pAudioStream->stop();
        }
        stopTimer(0);

    }

    void onCommand(UINT id ) {
        switch(id) {
            case ID_FILEOPEN:
                openFile();
                break;
            
            case ID_PLAY:
               play();
               break;

            case ID_STOP:
                stop();
                break;
        }
    }
    
    void openFile() {
        if (pVideoStream != NULL) {
            delete pVideoStream;
            pVideoStream = NULL;
        }
        if (pAudioStream != NULL) {
            delete pAudioStream;
            pAudioStream = NULL;
        }

        LPSTR fileName = getOpenFileName("AVI|*.avi");
        if (fileName != NULL) {
            if (aviFile.open(fileName)) {
                pAudioStream = aviFile.getAudioStream(this);
                if (pAudioStream == NULL) {
                    debugPrint("No audio found!");
                }
                else {
                    LONG audioDuration = pAudioStream->getDuration();
                    startTime = 0;
                    debugPrint("AudioDuration=%ld\n", audioDuration);
                    slider->setRange(0, audioDuration);
                }
                pVideoStream = aviFile.getVideoStream();
                if (pVideoStream == NULL) {
                    showMsg("Codec not supported!");
                }
                else {
                    currentFrame = 0;
                    Size sz = pVideoStream->getSize();
                    debugPrint("width=%d, height=%d\n", sz.width, sz.height);
                    debugPrint("VideoDuration=%ld\n", pVideoStream->getDuration());
                    frameCount = pVideoStream->getFrameCount();
                    debugPrint("frameCount=%ld\n", frameCount);
                    LONG frameRate = frameCount * 1000 / pVideoStream->getDuration();
                    debugPrint("rate=%d\n", frameRate);
                    if (pAudioStream == NULL) {
                        slider->setRange(0, frameCount);
                    }
                }

                panel->redraw();                

            }
            else {
                showMsg("fail to load file!");
            }
        }
    }
    AviFile aviFile;
    VideoStream* pVideoStream;
    AudioStream* pAudioStream;
    Panel* panel;
    SliderCtrl* slider;
    Menu menuBar;
    PopupMenu fileMenu;
    LONG currentFrame;
    LONG frameCount;
    LONG startTime;

    friend class MyPanel;
};


void MyPanel::onPaint(Graphic& gr) {
    MyFrame* frame = (MyFrame*)getParent();
    VideoStream* pVideoStream = frame->pVideoStream;
    AudioStream* pAudioStream = frame->pAudioStream;

    if (pVideoStream != NULL) {
        LONG curFrame = frame->currentFrame;
        if (pAudioStream != NULL) {
            curFrame = pVideoStream->getFrameFromTime(pAudioStream->getElapsedTime());
        }

        pVideoStream->drawFrame(gr, 0, 0, curFrame);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    Application app;
    MyFrame frame;

    return app.run(frame);
}
