#ifndef AVIFILE_H
#define AVIFILE_H

#include "types.h"
#include "vfw.h"

class VideoStream;
class AudioStream;
class CustCtrl;

class DllExport AviFile {
public:
    AviFile();
    ~AviFile();

    BOOL open(LPSTR strFileName);
    void close();

    VideoStream* getVideoStream();
    AudioStream* getAudioStream(CustCtrl* pCtrl);

private:
    PAVIFILE pFile;    
};

#endif