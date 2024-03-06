#include "WavePlayer.h"
#include "Window.h"

WavePlayer::WavePlayer()
{
    _isPlaying = FALSE;
    hWaveOut = NULL;
}

WavePlayer::~WavePlayer()
{
    close();
}

void WavePlayer::close()
{
    if (hWaveOut == NULL)
        return;

    stop();

    for (UINT i = 0; i < 2; i++)
    {
        LPWAVEHDR lpwh = &waveHeaders[i];

        waveOutUnprepareHeader(hWaveOut, lpwh, sizeof(WAVEHDR));
        free(lpwh->lpData);
    }

    waveOutClose(hWaveOut);
    hWaveOut = NULL;

}

BOOL WavePlayer::open(WavePlayerReader *pReader, CustCtrl *pCtrl, WAVEFORMATEX &format, LONG dataSize)
{
    close();

    this->pCtrl = pCtrl;
    this->pReader = pReader;
    this->dataSize = dataSize;
    dataToRead = dataSize;

    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &format, (DWORD)pCtrl->getHandle(), 0, CALLBACK_WINDOW) != 0) {
        debugPrint("waveOutOpen fail!\n");
        return FALSE;
    }

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
    dataToRead = dataSize - offset;
    _isPlaying = TRUE;

    for (UINT i = 0; i < 2; i++)
    {
        queueWaveData(&waveHeaders[i]);
    }
}

void WavePlayer::stop()
{
    _isPlaying = FALSE;
    waveOutReset(hWaveOut);
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

    if (_isPlaying && sizeToRead > 0)
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
    else if (_isPlaying)
    {
        _isPlaying = FALSE;
        debugPrint("Audio finish!\n");
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

void WavePlayer::getDeviceCaps(Vector<WAVEOUTCAPS>& devices)
{    
    WAVEOUTCAPS woc;
    UINT iNumDevs = waveOutGetNumDevs();

    for (UINT i = 0; i < iNumDevs; i++)
    {
        if (!waveOutGetDevCaps(i, &woc, sizeof(WAVEOUTCAPS)))
        {
            devices.add(woc);
            //debugPrint("Device ID #%u: %s, wChannels=%d, dwFormats=%04X, dwSupport=%04X\r\n", i, woc.szPname, woc.wChannels, woc.dwFormats, woc.dwSupport);
        }
    }
}

 void WavePlayer::setVolume(WORD volume)
 {
    waveOutSetVolume(hWaveOut, MAKELONG(volume, volume));
 }