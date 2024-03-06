#include "VideoStream.h"
#include "Graphic.h"

VideoStream::VideoStream(PAVISTREAM pStream, PGETFRAME pFrame)
{
    this->pStream = pStream;
    this->pFrame = pFrame;

    hDrawDib = DrawDibOpen();
}

VideoStream::~VideoStream()
{
    DrawDibClose(hDrawDib);

	AVIStreamGetFrameClose(pFrame);
	AVIStreamRelease(pStream);
}

Size VideoStream::getSize()
{
    AVISTREAMINFO si;
    AVIStreamInfo(pStream, &si, sizeof(si));
    Rect rc = *(Rect*) &si.rcFrame;
	return Size(rc.getWidth(), rc.getHeight());
}

void VideoStream::drawFrame(Graphic &graphic, int x, int y, LONG lFrame)
{
    Size sz = getSize();

    LPBITMAPINFOHEADER lpBi = (LPBITMAPINFOHEADER) AVIStreamGetFrame(pFrame, lFrame);

    DrawDibDraw(hDrawDib, graphic.getHandle(), x, y, sz.width, sz.height, lpBi,
        NULL, 0, 0, sz.width, sz.height, 0);
}

LONG VideoStream::getDuration()
{
	return AVIStreamSampleToTime(pStream, getFrameCount());
}

LONG VideoStream::getFrameCount()
{
    AVISTREAMINFO si;
    AVIStreamInfo(pStream, &si, sizeof(si));
    return si.dwLength;
}

LONG VideoStream::getFrameFromTime(LONG lTime)
{
	return AVIStreamTimeToSample(pStream, lTime);
}

LONG VideoStream::getTimeFromFrame(LONG lFrame)
{
	return AVIStreamSampleToTime(pStream, lFrame);
}