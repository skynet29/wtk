#ifndef WAVEPLAYER_H
#define WAVEPLAYER_H

#include "types.h"
#include "Vector.h"

class CustCtrl;

class DllExport WavePlayerReader {
public:
    virtual LONG readData(LPSTR buff, LONG bufSize) = 0;
};

class DllExport WavePlayer {
public:
    WavePlayer();
    ~WavePlayer();
    
    BOOL open(WavePlayerReader* pReader, CustCtrl* pCtrl, WAVEFORMATEX& format, LONG dataSize);
    void close();
    void play(LONG offset);
    void stop();
    LONG getSamplePlayed();
    BOOL isPlaying() {return _isPlaying;}

    static void getDeviceCaps(Vector<WAVEOUTCAPS>& devices);


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