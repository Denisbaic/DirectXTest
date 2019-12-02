// DirectXTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
int main()
{
	WNDCLASSEXW wnd;

	wnd.cbSize = sizeof(WNDCLASSEXW);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = nullptr;
	wnd.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wnd.hIconSm = wnd.hIcon;
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
	wnd.lpszMenuName = nullptr;
	wnd.lpszClassName = L"helloWorld";
	wnd.cbSize = sizeof(WNDCLASSEX);
	if (!RegisterClassEx(&wnd))
	{
		std::cout << "can't to register window";
		return 0;
	}
	RECT rect = { 0, 0, 800, 480};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	long lwidth = rect.right - rect.left;
	long lheight = rect.bottom - rect.top;

	long lleft = 0;
	long ltop = 0;
	HWND m_hwnd = CreateWindowEx(NULL, L"helloWorld", L"Простое приложение", WS_OVERLAPPEDWINDOW | WS_VISIBLE, lleft, ltop, lwidth, lheight, nullptr, nullptr, nullptr, nullptr);
	if (!m_hwnd)
	{
		std::cout << "can't to create window";
		return 0;
	}
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);
	MSG msg = { nullptr };
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);        
		DispatchMessage(&msg);
	}
    std::cout << "Hello World!\n";
	
	return int(msg.wParam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, nMsg, wParam, lParam);
	}
	return 0;
}
