#include "Sound.h"
#include "comdef.h"
#include <sstream>
#include <cassert>


void Sound::CreateSound()
{
	pSourceVoice->Start();

	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = 2 * 5 * 44100;
	buffer.pAudioData = soundData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 5 * 44100;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);
}

void Sound::Pause()
{
	pSourceVoice->Stop();
	pSourceVoice->FlushSourceBuffers();
}

Sound::Sound(): hWnd(nullptr)
{
	LPVOID Initilize = nullptr;
	CoInitialize(Initilize);
	HRESULT hr = XAudio2Create(&pXAudio2);
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "XAudio2Create failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		//std::cout << "XAudio2Create failure: " << err.ErrorMessage();
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}

	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "CreateMasteringVoice failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		//std::cout << "CreateMasteringVoice failure: " << err.ErrorMessage();
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
	hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &waveformat);
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "CreateSourceVoice failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}

	hr = pSourceVoice->Start();
	if (FAILED(hr))
	{
		_com_error err(hr);
		std::ostringstream oss;

		oss << "Start failure: " << err.ErrorMessage();
		MessageBox(nullptr, oss.str().c_str(), "Opyat eta hernya ne rabotaet blya", MB_OK | MB_ICONEXCLAMATION);
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
	for (int index = 0, second = 0; second < 5; second++)
	{
		for (int cycle = 0; cycle < 441; cycle++)
		{
			for (int sample = 0; sample < 100; sample++)
			{
				short value = sample < 50 ? 32767 : -32768;
				soundData[index++] = value & 0xFF;
				soundData[index++] = (value >> 8) & 0xFF;
			}
		}
	}
}

/*
Sound::XAudioDll::XAudioDll()
{
	LoadType type = LoadType::System;
	while (true)
	{
		hModule = LoadLibrary(LPCSTR(GetDllPath(type)));
		if (hModule != 0)
		{
			return;
		}
		else
		{
			switch (type)
			{
			case LoadType::System:
				type = LoadType::Folder;
				break;
			case LoadType::Folder:
				type = LoadType::Local;
				break;
			case LoadType::Local:
				break;
			default:
				assert(false && "Bad LoadType encountered in XAudio Dll loading sequence loop");
			}
		}
	}
}

Sound::XAudioDll::~XAudioDll()
{
	if (hModule != 0)
	{
		FreeLibrary(hModule);
		hModule = 0;
	}
}


const wchar_t* Sound::XAudioDll::GetDllPath(LoadType type)
{
	switch (type)
	{
	case LoadType::System:
		return systemPath;
	case LoadType::Folder:
		return folderPath;
	case LoadType::Local:
		return localPath;
	default:
		assert(false && "Bad LoadType in GetDllPath function");
		return nullptr;
	}
}
*/