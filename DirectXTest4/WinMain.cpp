
#include <Windows.h>
#include "WindowsMessageMap.h"
#include "Window.h"
#include <sstream>


int messageLoop();
Window* g_point;
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR  lpCmdLine,int nCmdShow)
{
	try
	{
		Window wnd(800, 300, "My window");
		g_point = &wnd;
		return messageLoop();
	}
	catch (const Exception& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch(const std::exception& e)
	{
		MessageBox(nullptr,e.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch(...)
	{
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}

int messageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			const auto e_kb = g_point->kbd.ReadKey();
			if(e_kb.GetCode()==VK_F1 && e_kb.IsRelease())
			{
				g_point->sound.Pause();
			}
			else if(e_kb.GetCode() == VK_F1 && e_kb.IsPress())
			{
				g_point->sound.CreateSound();
			}
			while(!g_point->mouse.IsEmpty())
			{
				const auto e = g_point->mouse.Read();
				if(e.GetType()== Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position: (" << e.GetPosX() << "," << e.GetPosY() << ")";
					g_point->SetTitle(oss.str());
				}
			}
		}
		else
		{
			//run game code
			//UpdateScene();
			//RenderScene();
		}
	}
	return msg.wParam;
}