#ifndef WAVEPLAYER_H
#define WAVEPLAYER_H

#include "types.h"

class CustCtrl;

class DllExport WavePlayerReader {
public:
    virtual LONG readData(LPSTR buff, LONG bufSize) = 0;
    virtual void seek(LONG offset) = 0;
};

class DllExport WavePlayer {
public:
    WavePlayer();
    
    BOOL open(WavePlayerReader* pReader, CustCtrl* pCtrl, WAVEFORMATEX& format, LONG dataSize);
    void play(LONG offset);
    void stop();
    LONG getSamplePlayed();
    BOOL isPlaying() {return _isPlaying;}

private:
    void onWomDone(LPWAVEHDR lpwh);
    long queueWaveData(LPWAVEHDR lpwh);

    HWAVEOUT hWaveOut;
    DWORD bufSize;
    WAVEHDR waveHeaders[2];
    WavePlayerReader* pReader;
    LONG dataSize;
    LONG dataToRead;
    BOOL _isPlaying;
    CustCtrl* pCtrl;
    friend class CustCtrl;
};

#endif