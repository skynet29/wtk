#include "AudioStream.h"

AudioStream::AudioStream(CustCtrl* pCtrl, PAVISTREAM pStream)
{
    this->pStream = pStream;

    LONG cbFormat;
	AVIStreamFormatSize(pStream, 0, &cbFormat);
	LPWAVEFORMATEX lpFormat = (LPWAVEFORMATEX) malloc(cbFormat);
	AVIStreamReadFormat(pStream, 0, lpFormat, &cbFormat);
    debugPrint("nChannels=%d, nSamplesPerSec=%d, nAvgBytesPerSec=%d, wBitsPerSample=%d\n",
           lpFormat->nChannels,
           lpFormat->nSamplesPerSec,
           lpFormat->nAvgBytesPerSec,
           lpFormat->wBitsPerSample);

    samplePerSec = lpFormat->nSamplesPerSec;

    AVISTREAMINFO si;
    AVIStreamInfo(pStream, &si, sizeof(si));
    debugPrint("nbSamples=%ld\n", si.dwLength);
    sampleSize = si.dwSampleSize;
    LONG dataSize = si.dwLength * sampleSize;
    debugPrint("dataSize=%ld\n", dataSize);

    if (!player.open(this, pCtrl, *lpFormat, dataSize)) {
        debugPrint("WavePlayer.open fail!\n");
    }
    free(lpFormat);
}

AudioStream::~AudioStream()
{
	AVIStreamRelease(pStream);
}

LONG AudioStream::readData(LPSTR buff, LONG bufSize)
{
    LONG nbSampleRead;
    LONG nbBytesRead;
    LONG nbSampleToRead = bufSize / sampleSize;

    AVIStreamRead(pStream, currentSample, nbSampleToRead,
        buff, 
        bufSize,
        &nbBytesRead,
        &nbSampleRead);

    currentSample += nbSampleRead;

    //debugPrint("AudioStream::readData nbBytesRead=%ld\n", nbBytesRead);

    return nbBytesRead;
}

LONG AudioStream::getElapsedTime()
{
    return startTimeMs + MulDiv(player.getSamplePlayed(), 1000, samplePerSec);
}

LONG AudioStream::getDuration()
{
    AVISTREAMINFO si;
    AVIStreamInfo(pStream, &si, sizeof(si));
	return AVIStreamSampleToTime(pStream, si.dwLength);
}

void AudioStream::play(LONG startTimeMs)
{
    this->startTimeMs = startTimeMs;

	currentSample = AVIStreamTimeToSample(pStream, startTimeMs);

    LONG offset = currentSample * sampleSize;
    player.play(offset);
}

void AudioStream::stop()
{
    player.stop();
}

BOOL AudioStream::isPlaying()
{
    return player.isPlaying();
}

 void AudioStream::setVolume(WORD volume)
 {
    player.setVolume(volume);
 }