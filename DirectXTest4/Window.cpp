#include "Window.h"
#include <sstream>
#include <winuser.h>
#include "resource.h"
//Exception stuff
Window::WindowException::WindowException(int line, const char * file, HRESULT hr) noexcept: Exception(line,file),hr(hr)
{}

const char * Window::WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * Window::WindowException::GetType() const noexcept
{
	return "Window Exception";
}

std::string Window::WindowException::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if(nMsgLen==0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}
HRESULT Window::WindowException::GetErrorCode() const noexcept
{
	return hr;
}
std::string Window::WindowException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;
const char * Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

inline Window::WindowClass::WindowClass() noexcept: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wnd;
	wnd.hInstance = GetInstance();
	wnd.cbSize = sizeof(wnd);

	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = HandleMsgSetup;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;

	wnd.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wnd.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.hbrBackground = nullptr;
	wnd.lpszMenuName = nullptr;
	wnd.lpszClassName = GetName();
	
	RegisterClassEx(&wnd);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

Window::Window(int width, int height, const char * name):width(width),height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if(FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
	{
		throw CHWND_EXCEPT_LAST();
	}

	
	

	hWnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MAXIMIZEBOX | WS_SYSMENU, wr.left,
		wr.top, wr.right-wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);
	if(hWnd ==nullptr)
	{
		throw CHWND_EXCEPT_LAST();
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string title) noexcept
{
	if(SetWindowText(hWnd,title.c_str())==0)
	{
		throw CHWND_EXCEPT_LAST();
	}
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	if(nMsg==WM_CREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		const auto pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hwnd, nMsg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hwnd, nMsg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hwnd, nMsg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (nMsg)
	{
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/**************************Mouse message*****************************/
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMouseMove(pt.x, pt.y);
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEHWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if(GET_WHEEL_DELTA_WPARAM(wParam)>0)
		{
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if(GET_WHEEL_DELTA_WPARAM(wParam)<0)
		{
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	}
	/********************************************************************/
	/************************Keyboard message****************************/
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}		
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	/********************************************************************/
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, nMsg, wParam, lParam);
	}
	return 0;
}