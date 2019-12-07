
#include <Windows.h>
#include "WindowsMessageMap.h"
#include "Window.h"
#include <sstream>


int messageLoop();
Window* g_point;

int PianoState=0;
void MakePianoSound(Keyboard::Event const & e_kb);

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
			MakePianoSound(e_kb);
			
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
	}
	return msg.wParam;
}

void MakePianoSound(Keyboard::Event const & e_kb)
{
	if (e_kb.GetCode() == 0x5A)//Z
		PianoState = 0;
	else if (e_kb.GetCode() == 0x58)//X
		PianoState = 1;
	else if (e_kb.GetCode() == 0x43)//C
		PianoState = 2;
	else if (e_kb.GetCode() == 0x56)//V
		PianoState = 3;
	else if (e_kb.GetCode() == 0x42)//B
		PianoState = 4;
	else if (e_kb.GetCode() == 0x42)//N
		PianoState = 5;
	else if (e_kb.GetCode() == 0x4E)//M
		PianoState = 6;
	else if (e_kb.GetCode() == 0xBC)//,
		PianoState = 7;
	else if (e_kb.GetCode() == 0xBE)//.
		PianoState = 8;


	switch (PianoState)
	{
	case 0:
		switch (e_kb.GetCode())
		{
		case 0x41://A
			if(e_kb.IsPress())
				g_point->sound.CreateSound(0);
			else if(e_kb.IsRelease())
				g_point->sound.Pause(0);
			break;
		case 0x57://W
			if (e_kb.IsPress())
				g_point->sound.CreateSound(1);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(1);
			break;
		case 0x53://S
			if (e_kb.IsPress())
				g_point->sound.CreateSound(2);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(2);
			break;
		}
		break;
	case 8:
		if(e_kb.GetCode()== 0xBA && e_kb.IsPress())//;
			g_point->sound.CreateSound(87);
		else if (e_kb.GetCode() == 0xBA && e_kb.IsRelease())
			g_point->sound.Pause(87);
		break;
	default:
		switch (e_kb.GetCode())
		{
		case 0x41://A
			if (e_kb.IsPress())
				g_point->sound.CreateSound(3+(PianoState-1)*12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(3 + (PianoState-1) * 12);
			break;
		case 0x57://W
			if (e_kb.IsPress())
				g_point->sound.CreateSound(4 + (PianoState -1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(4 + (PianoState -1) * 12);
			break;
		case 0x53://S
			if (e_kb.IsPress())
				g_point->sound.CreateSound(5 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(5 + (PianoState - 1) * 12);
			break;
		case 0x45://E
			if (e_kb.IsPress())
				g_point->sound.CreateSound(6 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(6 + (PianoState - 1) * 12);
			break;
		case 0x44://D
			if (e_kb.IsPress())
				g_point->sound.CreateSound(7 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(7 + (PianoState - 1) * 12);
			break;
		case 0x46://F
			if (e_kb.IsPress())
				g_point->sound.CreateSound(8 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(8 + (PianoState - 1) * 12);
			break;
		case 0x54://T
			if (e_kb.IsPress())
				g_point->sound.CreateSound(9 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(9 + (PianoState - 1) * 12);
			break;
		case 0x47://G
			if (e_kb.IsPress())
				g_point->sound.CreateSound(10 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(10 + (PianoState - 1) * 12);
			break;
		case 0x59://Y
			if (e_kb.IsPress())
				g_point->sound.CreateSound(11 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(11 + (PianoState - 1) * 12);
			break;
		case 0x48://H
			if (e_kb.IsPress())
				g_point->sound.CreateSound(12 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(12 + (PianoState - 1) * 12);
			break;
		case 0x55://U
			if (e_kb.IsPress())
				g_point->sound.CreateSound(13 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(13 + (PianoState - 1) * 12);
			break;
		case 0x4A://J
			if (e_kb.IsPress())
				g_point->sound.CreateSound(14 + (PianoState - 1) * 12);
			else if (e_kb.IsRelease())
				g_point->sound.Pause(14 + (PianoState - 1) * 12);
			break;
		}
	}
}