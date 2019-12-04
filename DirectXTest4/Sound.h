#pragma once
#include <wrl/client.h>
//#include <xaudio2.h>
#include "XAudio/XAudio2.h"
class Sound
{
	const int sampleRate = 44100;
	const int seconds = 5;
	const int channels = 2;
	const int samples = seconds * sampleRate;
	Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice * pMasteringVoice;
	IXAudio2SourceVoice * pSourceVoice;
	byte soundData[2 * 5 * 44100];
	/*
	class XAudioDll
	{
	private:
		enum class LoadType
		{
			Folder,
			Local,
			System,
			Invalid
		};
	public:
		XAudioDll();
		~XAudioDll();
		operator HMODULE() const;
	private:
		static const wchar_t* GetDllPath(LoadType type);
	private:
		HMODULE hModule = 0;
		inline static  WCHAR const *   systemPath = L"XAudio2_7.dll";
#ifdef _M_X64
		inline static  WCHAR const *  folderPath = L"XAudio\\XAudio2_7_64.dll";
		inline static  WCHAR const *  localPath = L"XAudio2_7_64.dll";
#else
		inline static  WCHAR const *  folderPath = L"XAudio\\XAudio2_7_32.dll";
		inline static  WCHAR const *  localPath = L"XAudio2_7_32.dll";
#endif
	};
	*/
public:
	//XAudioDll xaudio_dll;
	HWND hWnd;
	void CreateSound();
	void Pause();
	Sound();
	~Sound()=default;
};

