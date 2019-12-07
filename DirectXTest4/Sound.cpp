#include "Sound.h"
#include "comdef.h"
#include <sstream>
#include <cassert>

void Sound::CreateSound(int kIndex)
{
	
	pSourceVoice[kIndex]->Start();
	
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = wavFILE[kIndex].dwWaveSize;
	buffer.pAudioData = wavFILE[kIndex].lpWaveData;
	//buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0;
	buffer.LoopCount = 0;
	
	HRESULT hr = pSourceVoice[kIndex]->SubmitSourceBuffer(&buffer);
}

void Sound::Pause(int kIndex)
{
	pSourceVoice[kIndex]->Stop();
	pSourceVoice[kIndex]->FlushSourceBuffers();
}

Sound::Sound(): hWnd(nullptr)
{
	//c_dsp_wav wavFILE[88];
	wavFILE[0].LoadWav("Wav/0/a0.wav");
	wavFILE[1].LoadWav("Wav/0/a-0.wav");
	wavFILE[2].LoadWav("Wav/0/b0.wav");

	for (int i=0;i<7;i++)
	{
		std::string a = "Wav/"+std::to_string(i + 1) +"/a" + std::to_string(i+1) + ".wav";
		std::string a_ = "Wav/" + std::to_string(i + 1)+"/a-" + std::to_string(i+1) + ".wav";
		std::string b = "Wav/" + std::to_string(i + 1)+"/b" + std::to_string(i+1) + ".wav";
		std::string c = "Wav/" + std::to_string(i + 1)+"/c" + std::to_string(i+1) + ".wav";
		std::string c_ = "Wav/" + std::to_string(i + 1)+"/c-" + std::to_string(i+1) + ".wav";
		std::string d = "Wav/" + std::to_string(i + 1)+"/d" + std::to_string(i+1) + ".wav";
		std::string d_ = "Wav/" + std::to_string(i + 1)+"/d-" + std::to_string(i+1) + ".wav";
		std::string e = "Wav/" + std::to_string(i + 1)+"/e" + std::to_string(i+1) + ".wav";
		std::string f = "Wav/" + std::to_string(i + 1)+"/f" + std::to_string(i+1) + ".wav";
		std::string f_ = "Wav/" + std::to_string(i + 1)+"/f-" + std::to_string(i + 1) + ".wav";
		std::string g = "Wav/" + std::to_string(i + 1)+"/g" + std::to_string(i+1) + ".wav";
		std::string g_ = "Wav/" + std::to_string(i + 1)+"/g-" + std::to_string(i + 1) + ".wav";

		wavFILE[3+i*12].LoadWav(a.c_str());
		wavFILE[4 + i * 12].LoadWav(a_.c_str());
		wavFILE[5 + i * 12].LoadWav(b.c_str());
		wavFILE[6 + i * 12].LoadWav(c.c_str());
		wavFILE[7 + i * 12].LoadWav(c_.c_str());
		wavFILE[8 + i * 12].LoadWav(d.c_str());
		wavFILE[9 + i * 12].LoadWav(d_.c_str());
		wavFILE[10 + i * 12].LoadWav(e.c_str());
		wavFILE[11 + i * 12].LoadWav(f.c_str());
		wavFILE[12 + i * 12].LoadWav(f_.c_str());
		wavFILE[13 + i * 12].LoadWav(g.c_str());
		wavFILE[14 + i * 12].LoadWav(g_.c_str());
	}
	wavFILE[87].LoadWav("Wav/8/c8.wav");

	LPVOID Initilize = nullptr;
	CoInitialize(Initilize);
	HRESULT hr = XAudio2Create(&pXAudio2);
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "XAudio2Create failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}

	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "CreateMasteringVoice failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}

	
	WAVEFORMATEX waveformat;
	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nChannels = 1;
	waveformat.nSamplesPerSec = 44100;
	waveformat.nAvgBytesPerSec = 44100 * 2;
	waveformat.nBlockAlign = 2;
	waveformat.wBitsPerSample = 16;
	waveformat.cbSize = 0;
	
	for(int i=0;i<88;i++)
	{
		pXAudio2->CreateSourceVoice(&pSourceVoice[i], wavFILE[i].lpWaveHeader);
	}
	
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "CreateSourceVoice failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
}


void Sound::c_dsp_wav::reset()
{
	lpBuf = NULL;
	lpWaveHeader = NULL;
	lpWaveData = NULL;
	dwFileLength = 0;
	dwWaveSize = 0;
	dwPlayPos = 0;
	dwWritePos = 0;
}

Sound::c_dsp_wav::c_dsp_wav()
{
	reset();
}

Sound::c_dsp_wav::~c_dsp_wav()
{
	free(lpBuf);
	reset();
}

BOOL Sound::c_dsp_wav::LoadWav(char const *fPath)
{
	FILE * f;
	if ((f = fopen(fPath, "rb")) != 0)
	{
		fseek(f, 0, SEEK_END);
		dwFileLength = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (!(lpBuf = malloc(dwFileLength)))
			return FALSE;
		fread(lpBuf, 1, dwFileLength, f);
		fclose(f);

		if (!DSParseWaveResource(lpBuf, &lpWaveHeader, &lpWaveData, &dwWaveSize))
			return FALSE;

		dwWaveSize = dwWaveSize / ((lpWaveHeader->wBitsPerSample == 8) ? 1 : 2);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL Sound::DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader,
	BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
	DWORD *pdw;
	DWORD *pdwEnd;
	DWORD dwRiff;
	DWORD dwType;
	DWORD dwLength;

	if (ppWaveHeader)
		*ppWaveHeader = NULL;

	if (ppbWaveData)
		*ppbWaveData = NULL;

	if (pcbWaveSize)
		*pcbWaveSize = 0;

	pdw = (DWORD *)pvRes;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;

	if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
		goto exit;      // not even RIFF
	if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
		goto exit;      // not a WAV
	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength - 4);

	while (pdw < pdwEnd)
	{
		dwType = *pdw++;
		dwLength = *pdw++;

		switch (dwType)
		{
		case mmioFOURCC('f', 'm', 't', ' '):
			if (ppWaveHeader && !*ppWaveHeader)
			{
				if (dwLength < sizeof(WAVEFORMAT))
					goto exit;      // not a WAV

				*ppWaveHeader = (WAVEFORMATEX *)pdw;

				if ((!ppbWaveData || *ppbWaveData) &&
					(!pcbWaveSize || *pcbWaveSize))
				{
					return TRUE;
				}
			}
			break;
		case mmioFOURCC('d', 'a', 't', 'a'):
			if ((ppbWaveData && !*ppbWaveData) ||
				(pcbWaveSize && !*pcbWaveSize))
			{
				if (ppbWaveData)
					*ppbWaveData = (LPBYTE)pdw;

				if (pcbWaveSize)
					*pcbWaveSize = dwLength;

				if (!ppWaveHeader || *ppWaveHeader)
					return TRUE;
			}
			break;
		}
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength + 1)&~1));
	}
exit:
	return FALSE;
}
