#pragma once
#include "Win.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"

class Window
{
public:
	class WindowException : public Exception
	{
	public:
		WindowException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static  std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class  WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static  constexpr  const char* wndClassName = "Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string) noexcept;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
	Sound sound;
private:
	int width{};
	int height{};
	HWND hWnd;
};

//Error macro
#define  CHWND_EXCEPT(hr) Window::WindowException(__LINE__,__FILE__,hr);
#define  CHWND_EXCEPT_LAST() Window::WindowException(__LINE__,__FILE__,GetLastError());