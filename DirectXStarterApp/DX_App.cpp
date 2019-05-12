
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
			static_cast<UINT>(ceil(1024.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(768.f * dpiY / 96.f)),
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



HRESULT DX_App::CreateDrawTextResources()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(m_hwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	//DrawText Tutorial
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory_)
		);
	}
	wszText_ = L"Hello World using  DirectWrite!";
	cTextLength_ = (UINT32)wcslen(wszText_);

	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory_->CreateTextFormat(
			L"Gabriola",                // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			62.0f,
			L"en-us",
			&pTextFormat_
		);
	}
	// Center align (horizontally) the text.
	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	

	if (!m_pRenderTarget)
	{
		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd,size),
			&m_pRenderTarget
		);


		// Create a black brush.
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&pBlackBrush_
			);
		}
	}
	//DrawText Tutorial
	return hr;
}



HRESULT DX_App::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}

HRESULT DX_App::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);
		// Create a Direct2D render target
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr))
		{
			// Create a gray brush
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
			);
		}

		if (SUCCEEDED(hr))
		{
			//Create a blue brush
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
			);
		}

		//DrawText Source
		//********************************
		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&pDWriteFactory_)
			);
		}
		wszText_ = L"PLANETARY SCAN\n\nEnergy: variable\nScience: variable\n\n Orders, Captain?\n1.Replenish Energy\n2.Gather Science\n3.Leave Orbit";
		cTextLength_ = (UINT32)wcslen(wszText_);

		if (SUCCEEDED(hr))
		{
			hr = pDWriteFactory_->CreateTextFormat(
				L"Monospace",                // Font family name.
				NULL,                       // Font collection (NULL sets it to use the system font collection).
				DWRITE_FONT_WEIGHT_BOLD,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				45.0f,
				L"en-us",
				&pTextFormat_
			);
		}
		// Center align (horizontally) the text.
		if (SUCCEEDED(hr))
		{
			hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		}

		if (SUCCEEDED(hr))
		{
			hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}


		
			//// Create a Direct2D render target.
			//hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			//	D2D1::RenderTargetProperties(),
			//	D2D1::HwndRenderTargetProperties(m_hwnd, size),
			//	&m_pRenderTarget
			//);


			// Create a black brush.
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Black),
					&pBlackBrush_
				);
			}
		

		//********************************
		//DrawText Source


		
	}

	return hr;

}

// Release the render target and brushes
void DX_App::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&pRT_);
	SafeRelease(&pBlackBrush_);
}

LRESULT CALLBACK DX_App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DX_App *pDX_App = (DX_App *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pDX_App)
		);

		result = 1;
	}
	else
	{
		DX_App *pDX_App = reinterpret_cast<DX_App *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hWnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDX_App)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDX_App->OnReSize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pDX_App->OnRender();
				//pDX_App->RenderDrawText();
				ValidateRect(hWnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;

			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}

	}

	return result;
}







HRESULT DX_App::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		//m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->SetTransform(D2D1::IdentityMatrix()); // Text
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		//Draw a grid background
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		/*for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
			);
		}
*/
		// Draw two rectangles.
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - 50.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f
		);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - 100.0f,
			rtSize.height / 2 - 100.0f,
			rtSize.width / 2 + 100.0f,
			rtSize.height / 2 + 100.0f
		);

		// Draw a filled rectangle
		//m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);
		// Draw the outline of a rectangle.
		//m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);


		//DrawText
		// ********************************
		FLOAT dpiX, dpiY;
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		RECT rc;
		GetClientRect(m_hwnd, &rc);




		/*D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left) / dpiX,
			static_cast<FLOAT>(rc.top) / dpiY,
			static_cast<FLOAT>(rc.right - rc.left) / dpiX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiY
		);*/

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(1000),
			static_cast<FLOAT>(700),
			static_cast<FLOAT>(rc.right - rc.left) / dpiX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiY
		); 



		m_pRenderTarget->DrawText(
			wszText_,			// The string to render.
			cTextLength_,		// The string's length.
			pTextFormat_,		// The text format.
			layoutRect,			// The region of the window where the text will be rendered.
			pBlackBrush_		// The brush used to draw the text.
		);

		// ********************************
		//DrawText



		hr = m_pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
		
	}
	

	

	return hr;
}


HRESULT DX_App::RenderDrawText()
{
	HRESULT hr = S_OK;
	hr = CreateDrawTextResources();

	FLOAT dpiX, dpiY;
	m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	RECT rc;
	GetClientRect(m_hwnd, &rc);

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left) / dpiX,
		static_cast<FLOAT>(rc.top) / dpiY,
		static_cast<FLOAT>(rc.right - rc.left) / dpiX,
		static_cast<FLOAT>(rc.bottom - rc.top) / dpiY
	);

	m_pRenderTarget->DrawText(
		wszText_,        // The string to render.
		cTextLength_,    // The string's length.
		pTextFormat_,    // The text format.
		layoutRect,       // The region of the window where the text will be rendered.
		pBlackBrush_     // The brush used to draw the text.
	);


	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Blue));

		// Call the DrawText method of this class.
		//hr = DrawText();

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->EndDraw(
			);
		}
	}

	// DrawTexT
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}


void DX_App::OnReSize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}



