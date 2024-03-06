#include "WaveFile.h"

WaveFile::WaveFile() 
{
    hMMIO = NULL;
}

WaveFile::~WaveFile()
{
    if (hMMIO != NULL)
        mmioClose(hMMIO, 0);
}

void WaveFile::play(LONG startTimeSec)
{
    this->startTimeSec = startTimeSec;

    LONG offset = startTimeSec * format.nAvgBytesPerSec;

    mmioSeek(hMMIO, offset + dataOffset, SEEK_SET);

    player.play(offset);
}

void WaveFile::stop()
{
    player.stop();
}

BOOL WaveFile::isPlaying()
{
    return player.isPlaying();
}

LONG WaveFile::getDuration()
{
    return dataSize / format.nAvgBytesPerSec;
}

LONG WaveFile::getElapsedTime()
{
    LONG samplePlayed = player.getSamplePlayed();
    //debugPrint("samplePlayed=%ld\n", samplePlayed);
    return startTimeSec + samplePlayed / format.nSamplesPerSec;
}

BOOL WaveFile::open(CustCtrl* pCtrl, LPSTR fileName)
{
    hMMIO = mmioOpen(fileName, 0, MMIO_READ);
    if (hMMIO == NULL)
        return FALSE;

    MMCKINFO infoParent;
    infoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    if (mmioDescend(hMMIO, &infoParent, 0, MMIO_FINDRIFF))
    {
        debugPrint("ERROR: This file doesn't contain a WAVE!\n");
        return FALSE;
    }

    debugPrint("WAVE off=%ld\n", infoParent.dwDataOffset);

    if (!readFormatChunk(infoParent))
        return FALSE;

    readInfoChunk(infoParent);    

    if (!readDataChunk(infoParent))
        return FALSE;

    if (!player.open(this, pCtrl, format, dataSize))
        return FALSE;

    return TRUE;
}

LONG WaveFile::readData(LPSTR buff, LONG bufSize)
{
    return mmioRead(hMMIO, buff, bufSize);
}

BOOL WaveFile::readDataChunk(MMCKINFO& infoParent)
{
    MMCKINFO chunk;
    chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hMMIO, &chunk, &infoParent, MMIO_FINDCHUNK))
    {
        debugPrint("ERROR: reading the data chunk!\n");
        return FALSE;
    }
    debugPrint("DATA off=%d\n", chunk.dwDataOffset);
    dataOffset = chunk.dwDataOffset;

    dataSize = chunk.cksize;
    debugPrint("nbBytesToPlay=%ld\n", dataSize);
    debugPrint("duration=%d\n", dataSize / format.nAvgBytesPerSec);

    //mmioAscend(hMMIO, &chunk, 0);    
    return TRUE;
}

BOOL WaveFile::readInfoItem(MMCKINFO& infoParent, char* name, StrBuffer& text)
{
    MMCKINFO chunk;
    chunk.ckid = mmioFOURCC(name[0], name[1], name[2], name[3]);
    if (mmioDescend(hMMIO, &chunk, &infoParent, MMIO_FINDCHUNK))
    {
        /* Oops! The required fmt chunk was not found! */
        debugPrint("ERROR: %s not found!\n", name);
        return FALSE;
    }

    debugPrint("%s off=%d size=%d\n", name, chunk.dwDataOffset, chunk.cksize);
    text.allocate(chunk.cksize);
    if (mmioRead(hMMIO, (HPSTR)text.getBuffer(), chunk.cksize) != (LRESULT)chunk.cksize) {
        debugPrint("Fail to read %s\n", name);
    }     
    debugPrint("%s=%s\n", name, text.getBuffer());
    mmioAscend(hMMIO, &chunk, 0);
    return TRUE;
      
}

BOOL WaveFile::readInfoChunk(MMCKINFO& infoParent) 
{
    MMCKINFO chunk;
    chunk.fccType = mmioFOURCC('I', 'N', 'F', 'O');
    if (mmioDescend(hMMIO, &chunk, &infoParent, MMIO_FINDLIST) != MMSYSERR_NOERROR)
    {
        debugPrint("No INFO found!\n");
        return FALSE;
    }

    debugPrint("INFO off=%d size=%d\n", chunk.dwDataOffset, chunk.cksize);
    readInfoItem(chunk, "IART", strArtist);
    readInfoItem(chunk, "ICRD", strDate);
    readInfoItem(chunk, "INAM", strTitle); 

    return TRUE; 

}

BOOL WaveFile::readFormatChunk(MMCKINFO& infoParent)
{
    MMCKINFO chunk;

    chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hMMIO, &chunk, &infoParent, MMIO_FINDCHUNK))
    {
        debugPrint("ERROR: Required fmt chunk was not found!\n");
        return FALSE;
    }
    debugPrint("FMT off=%d\n", chunk.dwDataOffset);

    if (mmioRead(hMMIO, (HPSTR)&format, chunk.cksize) != (LRESULT)chunk.cksize)
    {
        /* Oops! */
        debugPrint("ERROR: reading the fmt chunk!\n");
        return FALSE;
    }

    debugPrint("nChannels=%d, nSamplesPerSec=%d, nAvgBytesPerSec=%d, wBitsPerSample=%d\n",
           format.nChannels,
           format.nSamplesPerSec,
           format.nAvgBytesPerSec,
           format.wBitsPerSample);

    mmioAscend(hMMIO, &chunk, 0);
    return TRUE;
}