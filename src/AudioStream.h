#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include "WavePlayer.h"
#include <vfw.h>

class DllExport AudioStream : public WavePlayerReader {
public:
    ~AudioStream();

    void play(LONG startTimeSec = 0);
    void stop();
    LONG getDuration();
    LONG getElapsedTime();
    BOOL isPlaying();
private:
    AudioStream(CustCtrl* pCtrl, PAVISTREAM pStream);

    LONG readData(LPSTR buff, LONG bufSize);

    PAVISTREAM pStream;
    WavePlayer player;
    LONG currentSample;
    UINT sampleSize;
    LONG startTimeMs;
    LONG samplePerSec;

    friend class AviFile;
};
 
#endif