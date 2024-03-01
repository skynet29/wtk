#include "WavePlayer.h"
#include "Window.h"


WavePlayer::WavePlayer()
{
    isPlaying = FALSE;
}

BOOL WavePlayer::open(WavePlayerReader *pReader, CustCtrl *pCtrl, WAVEFORMATEX &format, LONG dataSize)
{
    this->pCtrl = pCtrl;
    this->pReader = pReader;
    this->dataSize = dataSize;
    dataToRead = dataSize;

    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &format, (DWORD)pCtrl->getHandle(), 0, CALLBACK_WINDOW) != 0)
        return FALSE;

    bufSize = format.nAvgBytesPerSec * 2; // 2 sec

    for (UINT i = 0; i < 2; i++)
    {
        LPWAVEHDR lpwh = &waveHeaders[i];

        lpwh->lpData = (LPSTR)malloc(bufSize);
        lpwh->dwBufferLength = bufSize;
        lpwh->dwFlags = WHDR_DONE;
        lpwh->dwUser = (DWORD)this;

        MMRESULT ret = waveOutPrepareHeader(hWaveOut, lpwh, sizeof(WAVEHDR));
        if (ret != 0)
        {
            free(lpwh->lpData);
            break;
        }
    }

    return TRUE;
}

void WavePlayer::play(LONG offset)
{
    pReader->seek(offset);
    dataToRead = dataSize - offset;
    isPlaying = TRUE;

    for (UINT i = 0; i < 2; i++)
    {
        queueWaveData(&waveHeaders[i]);
    }
}

LONG WavePlayer::getSamplePlayed()
{
    MMTIME mmtime;

    mmtime.wType = TIME_SAMPLES;
    waveOutGetPosition(hWaveOut, &mmtime, sizeof(mmtime));

    return (mmtime.wType == TIME_SAMPLES) ? mmtime.u.sample : 0;
}

long WavePlayer::queueWaveData(LPWAVEHDR lpwh)
{
    LONG sizeToRead = min(bufSize, dataToRead);
    debugPrint("sizeToRead=%ld, dataToRead=%ld\n", sizeToRead, dataToRead);

    if (isPlaying && sizeToRead > 0)
    {
        LONG bytesRead = pReader->readData(lpwh->lpData, sizeToRead);
        if (bytesRead != sizeToRead)
        {
            debugPrint("ERROR: reading WAVE data!\n");
            return -1;
        }
        dataToRead -= bytesRead;
        lpwh->dwBufferLength = sizeToRead;

        waveOutWrite(hWaveOut, lpwh, sizeof(WAVEHDR));
    }
    else if (isPlaying) {
        isPlaying = FALSE;
        waveOutReset(hWaveOut);
        pCtrl->onAudioEndReached();
    }

    return 0;
}

void WavePlayer::onWomDone(LPWAVEHDR lpwh)
{
    debugPrint("onWomDone\n");
    queueWaveData(lpwh);
}