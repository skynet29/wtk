#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include "types.h"
#include <vfw.h>

class Graphic;

class DllExport VideoStream
{
public:
    ~VideoStream();

    Size getSize();
    LONG getDuration();
    LONG getFrameCount();
    LONG getFrameFromTime(LONG lTime);
    LONG getTimeFromFrame(LONG lFrame);

    void drawFrame(Graphic &graphic, int x, int y, LONG lFrame);
private:
    VideoStream(PAVISTREAM pStream, PGETFRAME pFrame);


	PAVISTREAM pStream;
	PGETFRAME pFrame;
	HDRAWDIB hDrawDib;

    friend class AviFile;
};

#endif