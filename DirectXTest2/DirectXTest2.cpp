// DirectXTest2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <Windows.h>
// Ширина и высота окна 
#define WINDOW_WIDTH  640 
#define WINDOW_HEIGHT 480 
HWND						  g_hWnd;
HINSTANCE					  g_hInstance;
D3D_DRIVER_TYPE				  g_driverType = D3D_DRIVER_TYPE_NULL;
ID3D11Device*				  g_pd3dDevice;
ID3D11DeviceContext*		  g_d3d11DevCon;
IDXGISwapChain*				  g_pSwapChain ;
ID3D11RenderTargetView*		  g_renderTarget ;
IDXGIFactory*				  g_pdxdFactory;
bool						  g_InitDone;
//////////////////////////////TEST///////////////////////////////////
float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;
////////////////////////////////////////////////////////////////////
HRESULT             InitWindow(); 
HRESULT             InitDirect3D12(); 
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); 
void                Cleanup(); 
void                RenderScene();
void				UpdateScene();
HRESULT				ResizeBuffers();
int					messageLoop();
int main()
{
	if (FAILED(InitWindow()))
		return 0;
	g_InitDone = true;
	if (FAILED(InitDirect3D12()))
	{
		Cleanup();
		return 0;
	}
	///////////////////////////////
	
	////////////////////////////////////
	messageLoop();
	/*
	MSG msg = { nullptr };
	while (WM_QUIT!=msg.message)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			RenderScene();
		}
	}
	*/
	std::cout << "Hello World!\n";
	Cleanup();
	return 0;
}
HRESULT InitWindow()
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
		return E_FAIL;
	}
	RECT rect = { 0, 0, 800, 480 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	long lwidth = rect.right - rect.left;
	long lheight = rect.bottom - rect.top;

	long lleft = 0;
	long ltop = 0;
	g_hWnd = CreateWindowEx(NULL, L"helloWorld", L"Простое приложение", WS_OVERLAPPEDWINDOW | WS_VISIBLE, lleft, ltop, lwidth, lheight, nullptr, nullptr, g_hInstance, nullptr);
	if (!g_hWnd)
	{
		std::cout << "can't to create window";
		return E_FAIL;
	}
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	return S_OK;
}
HRESULT InitDirect3D12()
{
	HRESULT hr;
	// Размеры клиентской области окна
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Список возможных типов устройства
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = sizeof driverTypes / sizeof driverTypes[0];
	for (UINT driverTypeIndex = 0; driverTypeIndex <= numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		//Create our SwapChain
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &g_pSwapChain, &g_pd3dDevice, NULL, &g_d3d11DevCon);
		if(SUCCEEDED(hr))
			break;		
	}

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	if (FAILED(hr))
		return hr;
	//Create our Render Target
	hr = g_pd3dDevice->CreateRenderTargetView(BackBuffer, NULL, &g_renderTarget);
	BackBuffer->Release();
	if (FAILED(hr))
		return hr;
	//Set our Render Target
	g_d3d11DevCon->OMSetRenderTargets(1, &g_renderTarget, NULL);
	if (FAILED(hr))
		return hr;

	return hr;
	/*
	HRESULT hr = S_OK;
	// Размеры клиентской области окна
	RECT rc;
	GetClientRect( g_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top; 
	// Список возможных типов устройства
	D3D_DRIVER_TYPE driverTypes[] ={               
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE      
	};
	UINT numDriverTypes = sizeof driverTypes / sizeof driverTypes[0];
	DXGI_SWAP_CHAIN_DESC sd;

	ZeroMemory(&sd, sizeof sd);
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	for(UINT driverTypeIndex =0; driverTypeIndex<= numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, //IDXGI ADAPTER
			g_driverType,	//Driver type
			NULL,	//HMODULE software
			0,	//FLAGS
			NULL, //pFeatureLevel,
			0,
			D3D11_SDK_VERSION, 
			&sd, &g_pSwapChain, 
			&g_pd3dDevice,
			NULL, 
			&g_d3d11DevCon);
		if(SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;
	// Представление данных для буфера визуализации
	ID3D11Texture2D *pBackBuffer;
	// Получим доступ к вторичному буферу с индексом 0 
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&pBackBuffer));
	if (FAILED(hr))
		return hr;
	// Создадим представление данных
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_renderTarget);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	// Свяжем буфер визуализации с графическим конвейером
	g_d3d11DevCon->OMSetRenderTargets(1, &g_renderTarget, NULL);
	// Настроим область отображения 
	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_d3d11DevCon->RSSetViewports(1, &vp);
	 
	return S_OK;
	*/
}

void RenderScene()
{
	// Очищаем вторичный буфер
	//(компоненты красного, зеленого, синего, прозрачность) 
	float ClearColor[4] = { red, green, blue, 1.0f };
	g_d3d11DevCon->ClearRenderTargetView(g_renderTarget, ClearColor);



	g_pSwapChain->Present(0, 0);
}

void UpdateScene()
{
	//Update the colors of our scene
	
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
	
}

HRESULT ResizeBuffers()
{
	if (g_renderTarget) g_renderTarget->Release();
	if (g_pSwapChain) g_pSwapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
	if (FAILED(hr))
		return hr;
	//Create our Render Target
	hr = g_pd3dDevice->CreateRenderTargetView(BackBuffer, NULL, &g_renderTarget);
	BackBuffer->Release();
	if (FAILED(hr))
		return hr;
	//Set our Render Target
	g_d3d11DevCon->OMSetRenderTargets(1, &g_renderTarget, NULL);
	BOOL InFullScreen;
	UINT width = 0;
	UINT height = 0;
	// Определим, какой режим сейчас установлен
	g_pSwapChain->GetFullscreenState(&InFullScreen, NULL);
	if(!InFullScreen)
	{
		// Оконный режим:
		// определяем размеры клиентской области окна
		RECT rc;
		GetClientRect( g_hWnd, &rc );
		width = rc.right - rc.left; 
		height = rc.bottom - rc.top;   
	}
	else
	{
		// Полноэкранный режим:
		// определяем разрешение экрана
		width = GetSystemMetrics (SM_CXSCREEN);
		height = GetSystemMetrics (SM_CYSCREEN); 
	}
	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_d3d11DevCon->RSSetViewports(1, &vp);
	
#ifdef _DEBUG
	std::cout << "Height:" <<height <<" Width:"<<width<< std::endl;
	std::cout << "Resize is successful"<<std::endl;
#endif
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_KEYDOWN:
		if(wParam==VK_F1)
		{
			BOOL InFullState;
			g_pSwapChain->GetFullscreenState(&InFullState, NULL);
			g_pSwapChain->SetFullscreenState(!InFullState, NULL);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (g_InitDone) ResizeBuffers();
		break;
	default:
		return DefWindowProc(hwnd, nMsg, wParam, lParam);
	}
	return 0;
}
void Cleanup()
{
	if (g_pdxdFactory) g_pdxdFactory->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
	if (g_renderTarget) g_renderTarget->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}

int messageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//run game code
			UpdateScene();
			RenderScene();
		}
	}
	return msg.wParam;
}