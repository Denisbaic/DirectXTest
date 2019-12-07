#pragma once
#include <wrl/client.h>
//#include <xaudio2.h>
#include "XAudio/XAudio2.h"


class Sound
{
	Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	IXAudio2MasteringVoice * pMasteringVoice;

	
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
	class c_dsp_wav
	{
	public:
		void				*lpBuf;
		WAVEFORMATEX	    *lpWaveHeader;
		DWORD				 dwFileLength;
		DWORD				 dwWaveSize;
		BYTE				*lpWaveData;
		DWORD				dwPlayPos;
		DWORD				dwWritePos;

		void reset();
		c_dsp_wav();
		~c_dsp_wav();
		BOOL LoadWav(char const *fPath);
	};
	c_dsp_wav wavFILE[88];
	
	//XAudioDll xaudio_dll;
	//XAUDIO2_BUFFER AudioBuffer[88];
	
	IXAudio2SourceVoice* pSourceVoice[88];
	static BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader,BYTE **ppbWaveData, DWORD *pcbWaveSize);
	HWND hWnd;
	void CreateSound(int kIndex);
	void Pause(int kIndex);
	Sound();
	~Sound()=default;
};


typedef Sound::c_dsp_wav *LPDSPWAV;