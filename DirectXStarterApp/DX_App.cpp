
// Code used from: https://docs.microsoft.com/en-us/windows/desktop/Direct2D/direct2d-quickstart#part-2-implement-the-class-infrastructure


#include "DX_App.h"





DX_App::DX_App() : 
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL)				  
{
}

DX_App::~DX_App()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

void DX_App::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DX_App::Initialize()
{
	HRESULT hr;

	// Initialize device-independent resources, such 
	// as the Direct2D factory

	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class
		WNDCLASSEX wcex		= { sizeof(WNDCLASSEX) };
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= DX_App::WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= sizeof(LONG_PTR);
		wcex.hInstance		= HINST_THISCOMPOMPONENT;
		wcex.hbrBackground	= NULL;
		wcex.lpszMenuName	= "Direct X Starter Application";
		wcex.hCursor		= LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName	= "DX_App_Tutorial";

		RegisterClassEx(&wcex);

		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will 
		// use to create its owns windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		// Create the window
		m_hwnd = CreateWindow(
			"DX_APP_Tutorial",
			"DirectX Starter App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPOMPONENT,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;

}



HRESULT DX_App::CreateDeviceIndependentResources()
{
	return E_NOTIMPL;
}

HRESULT DX_App::CreateDeviceResources()
{
	return E_NOTIMPL;
}

void DX_App::DiscardDeviceResources()
{
}

HRESULT DX_App::OnRender()
{
	return E_NOTIMPL;
}

void DX_App::OnReSize(UINT width, UINT height)
{
}

LRESULT DX_App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}


