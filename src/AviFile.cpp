#include "AviFile.h"
#include "VideoStream.h"
#include "AudioStream.h"

AviFile::AviFile()
{
	AVIFileInit();
	pFile = NULL;
}

AviFile::~AviFile()
{
    close();

	AVIFileExit();

}

BOOL AviFile::open(LPSTR strFileName)
{
	close();

	HRESULT hr;
	hr = AVIFileOpen(&pFile, strFileName, OF_SHARE_DENY_WRITE, NULL);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

void AviFile::close()
{
	if (pFile)
	{
		AVIFileRelease(pFile);
		pFile = NULL;
	}
}

VideoStream* AviFile::getVideoStream()
{
	HRESULT hr;
    PAVISTREAM pStream;

	hr = AVIFileGetStream(pFile, &pStream, streamtypeVIDEO, 0);
	if (FAILED(hr))
		return NULL;

	PGETFRAME pFrame = AVIStreamGetFrameOpen(pStream, NULL);
	if (!pFrame)
	{
		AVIStreamRelease(pStream);
		return NULL;
	}

	return new VideoStream(pStream, pFrame);
}

AudioStream* AviFile::getAudioStream(CustCtrl* pCtrl)
{
	HRESULT hr;
    PAVISTREAM pStream;

    hr = AVIFileGetStream(pFile, &pStream, streamtypeAUDIO, 0);
	if (FAILED(hr))
		return NULL;

	return new AudioStream(pCtrl, pStream);
}