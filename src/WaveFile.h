#ifndef WAVEFILE_H
#define WAVEFILE_H

#include "StrBuffer.h"
#include "WavePlayer.h"

class DllExport WaveFile : public WavePlayerReader {
public:
    WaveFile();
    ~WaveFile();

    BOOL open(LPSTR fileName);
    void play(LONG startTimeSec = 0);
    void stop();
    LONG getDuration();
    LONG getElapsedTime();
    BOOL isPlaying();
    void setVolume(WORD volume);
    void setOnEndReached(Callback* cbk);

private:
    LONG readData(LPSTR buff, LONG bufSize);

    BOOL readFormatChunk(MMCKINFO& infoParent);
    BOOL readInfoItem(MMCKINFO& infoParent, char* name, StrBuffer& text);
    BOOL readInfoChunk(MMCKINFO& infoParent); 
    BOOL readDataChunk(MMCKINFO& infoParent);

    WavePlayer player;
    StrBuffer strArtist;
    StrBuffer strTitle;
    StrBuffer strDate;
    HMMIO hMMIO;
    WAVEFORMATEX format;
    LONG dataSize;
    LONG dataOffset;
    LONG startTimeSec;
};

#endif