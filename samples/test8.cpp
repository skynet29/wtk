

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

    MyFrame() : Frame("Test 8") {
        pVideoStream = NULL;
        pAudioStream = NULL;
        menuBar.addPopupMenu(fileMenu, "File");
        fileMenu.addItem("Open...")->setOnClick(CBK(MyFrame, openFile));
        fileMenu.addSeparator();
        fileMenu.addItem("Play")->setOnClick(CBK(MyFrame, play));
        fileMenu.addItem("Stop")->setOnClick(CBK(MyFrame, stop));
        setMenu(menuBar);
        addChild(panel = new MyPanel());
        addChild(slider = new SliderCtrl());

        panel->setBackColor(Color::GREEN);
    }
protected:
    void onSize(UINT width, UINT height) {
        slider->setBounds(Bounds(0, height - 25, width, 25));
        panel->setSize(width, height -25);
    }

    void onAudioEndReached(void* from) {
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

    void play(void* from) {
         if (pAudioStream != NULL) {
             pAudioStream->play(startTime);
             startTimer(0, 10);
         }
         else {
            LONG frameRate = pVideoStream->getFrameCount() * 1000 / pVideoStream->getDuration();
            startTimer(0, 1000 / frameRate);
         }
    }

    void stop(void* from) {
        if (pAudioStream != NULL) {
            startTime = pAudioStream->getElapsedTime();
            pAudioStream->stop();
        }
        stopTimer(0);

    }
    
    void openFile(void* from) {
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
                pAudioStream = aviFile.getAudioStream();
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
                    showMsg("Video Codec not supported!");
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
