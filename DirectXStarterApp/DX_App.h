#ifndef DX_APP_H
#define DX_APP_H

#include "Includes.h"


class DX_App
{



private:

	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;

	// DrawText
	IDWriteFactory* pDWriteFactory_;
	IDWriteTextFormat* pTextFormat_;
	const wchar_t* wszText_;
	UINT32 cTextLength_;
	ID2D1Factory* pD2DFactory_;
	ID2D1HwndRenderTarget* pRT_;
	ID2D1SolidColorBrush* pBlackBrush_;
	//DrawText resources
	HRESULT CreateDrawTextResources();
	//DrawText Rendering
	HRESULT RenderDrawText();

	// DrawText

	// Initialize device-independent resources
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent
	void DiscardDeviceResources();

	// Draw content
	HRESULT OnRender();
	

	// Resize the render target
	void OnReSize(
		UINT width,
		UINT height
	);

	// The windows procedure
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

public:
	DX_App();
	~DX_App();

	// Register the window class and call methods for instantiating
	HRESULT Initialize();
 
	// Process and dispatch messages
	void RunMessageLoop();

};



#endif // !DX_APP_H
