#pragma once

// windows
#include <windows.h>

// d2d
#include <d2d1.h>
#pragma comment(lib, "d2d1")

// text
#include <dwrite.h>
#pragma comment(lib, "dwrite")

// windows imaging
#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs")

// time
#pragma comment(lib, "winmm")

#include "Scene.h"
#include "GameState.h"


#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }

// mouse
#define DIMOUSE_LEFTBUTTON		0
#define DIMOUSE_RIGHTBUTTON		1
#define DIMOUSE_MIDDLEBUTTON	2
#define DIMOUSE_4BUTTON			3
#define DIMOUSE_5BUTTON			4
#define DIMOUSE_6BUTTON			5
#define DIMOUSE_7BUTTON			6
#define DIMOUSE_8BUTTON			7

#define WINDOW_WIDTH	1280
#define WINDOW_HEIGHT	720

#define CLASS_NAME	L"Othello"
#define WINDOW_NAME	L"Othello Game"


HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);


class Othello
{
public:
	Othello();
	~Othello();

private:
	HRESULT CreateAppResource();
	HRESULT CreateDeviceResource();

	void DiscardDeviceResource();

	// drawing(render) scenes
	void DrawTitleScene();
	void DrawSettingScene();
	void DrawNewGameScene();
	void DrawGameScene();
	void DrawPauseUI();
	void DrawFinishUI();

	// window
	void OnResize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	// initialize window
	HRESULT Initialize(HINSTANCE hInstance);

	// draw
	bool OnRender(float timeDelta);

private:
	// window
	HWND hwnd;

	// D2D factory & render target
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	// text
	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;

	// brushes
	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1SolidColorBrush* pTranslucentBrush;

	// geometries
	D2D1_ELLIPSE ellipse;
	ID2D1EllipseGeometry* pEllipseGeometry;

	// bitmap
	IWICImagingFactory* pWICFactory;
	ID2D1Bitmap* pBitmap;

	// Time
	float elapsedTime;
};

#pragma region Constructor & Destructor

// Initialize member variables
Othello::Othello() :
	hwnd(NULL),
	pD2DFactory(NULL),
	pRenderTarget(NULL),
	pBlackBrush(NULL),
	pTranslucentBrush(NULL),
	pEllipseGeometry(NULL),
	pDWriteFactory(NULL),
	pTextFormat(NULL),
	pWICFactory(NULL),
	pBitmap(NULL)
{
}

Othello::~Othello()
{
	DiscardDeviceResource();

	// release app resources
	// D2D Factory
	SAFE_RELEASE(pD2DFactory);
	// geometries
	SAFE_RELEASE(pEllipseGeometry);
	// text
	SAFE_RELEASE(pDWriteFactory);
	SAFE_RELEASE(pTextFormat);
	// bitmap
	SAFE_RELEASE(pWICFactory);
}

#pragma endregion

#pragma region Create App/Device resources & Discard device resources

HRESULT Othello::CreateAppResource()
{
	HRESULT hr = S_OK;

#pragma region Create D2D Factory

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) return hr;

#pragma endregion

#pragma region Create Text Factory & Format

	// create text factory
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
	if (FAILED(hr)) return hr;

	// create text format by factory
	hr = pDWriteFactory->CreateTextFormat(L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.f, L"", &pTextFormat);
	if (FAILED(hr)) return hr;

	// initialize text format
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

#pragma endregion

#pragma region Create Geometries

	// set structure (values of geometries) and create geometries
	ellipse = D2D1::Ellipse(D2D1::Point2F(0.f, 0.f), 30.f, 30.f);
	hr = pD2DFactory->CreateEllipseGeometry(ellipse, &pEllipseGeometry);
	if (FAILED(hr)) return hr;

#pragma endregion

#pragma region Create Bitmap Factory

	// COM library must be initialized before
	// create WIC Factory
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	if (FAILED(hr)) return hr;

#pragma endregion

#pragma region Initialize Physics

	//state.x = 100;
	//state.v = 0;

#pragma endregion

	//elapsedTime = 0;

	return hr;
}

HRESULT Othello::CreateDeviceResource()
{
	HRESULT hr = S_OK;

	// return if device resources are alive
	if (pRenderTarget) return hr;

#pragma region Create Render Target

	RECT rc;
	GetClientRect(hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size), &pRenderTarget);
	if (FAILED(hr)) return hr;

#pragma endregion

#pragma region Create Brushes

	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.f), &pBlackBrush);
	if (FAILED(hr)) return hr;	
	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 0.5f), &pTranslucentBrush);
	if (FAILED(hr)) return hr;

#pragma endregion

#pragma region Create Bitmaps

	hr = LoadBitmapFromFile(pRenderTarget, pWICFactory, L".\\campus.jpg", 0, 0, &pBitmap);
	if (FAILED(hr)) return hr;

#pragma endregion

	return hr;
}

void Othello::DiscardDeviceResource()
{
	// render target
	SAFE_RELEASE(pRenderTarget);
	// brushes
	SAFE_RELEASE(pBlackBrush);
	SAFE_RELEASE(pTranslucentBrush);

	// bitmaps
	SAFE_RELEASE(pBitmap);
}

#pragma endregion

bool Othello::OnRender(float timeDelta)
{
	HRESULT hr = CreateDeviceResource();
	if (FAILED(hr)) return false;

	pRenderTarget->BeginDraw();

	// initialize transform and color of render target
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	DrawTitleScene();

	//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(100, 100)));
	//pRenderTarget->DrawEllipse(ellipse, pBlackBrush);

	//pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	//const WCHAR text[] = L"Test";
	//pRenderTarget->DrawText(text, ARRAYSIZE(text) - 1, pTextFormat, D2D1::RectF(20, 20, 60, 40), pBlackBrush);

#pragma region Physics
	//
	//	// update physics state
	//
	//	integrate(state, elapsedTime, timeDelta);
	//	elapsedTime += timeDelta;
	//
	//	if (fabs(state.x) < 0.01f && fabs(state.v) < 0.01f)
	//	{
	//		// 물리 상태 리셋
	//		state.x = 100;
	//		state.v = 0;
	//		elapsedTime = 0;
	//	}
	//
	//	WCHAR strbuf[180];
	//
	//	D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
	//
	//	wsprintf(strbuf, L"elapsed time = %d\n x,v = %4d, %4d\n", (int)elapsedTime, (int)state.x, (int)state.v);
	//	pRenderTarget->DrawText(strbuf, (int)wcslen(strbuf), pTextFormat, D2D1::RectF(0, 0, renderTargetSize.width / 4, renderTargetSize.height / 8), pBlackBrush);
	//
	//	// draw after physics update
	//
	//	ellipse.point.x = 510.f + state.x * 4;
	//	ellipse.point.y = 240.f;
	//
	//	pRenderTarget->FillEllipse(ellipse, pBlackBrush);
	//
#pragma endregion

	hr = pRenderTarget->EndDraw();
	if (FAILED(hr)) return false;

	// if device is lost while running OnRender() function, discard all device resources
	if (hr == D2DERR_RECREATE_TARGET)
	{
		DiscardDeviceResource();
	}

	return true;
}

void Othello::OnResize()
{
	if (!pRenderTarget) return;

	RECT rc;
	GetClientRect(hwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	pRenderTarget->Resize(size);
}

#pragma region Scene Drawings

void Othello::DrawTitleScene()
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::ForestGreen));
	
	// draw wallpaper

	// draw title text

	// draw "New Game" button

	// draw "Setting" button

	// draw "Exit" button
}

void Othello::DrawSettingScene()
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// draw a horizontal line

	// draw return button

	// draw "Sound", "Display" button

	// if "Sound" selected,
	// draw sound settings

	// else if "Display" selected,
	// draw display settings
}

void Othello::DrawNewGameScene()
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	// draw "Start a new game" text

	// draw time limit settings

	// draw black settings

	// draw white settings

	// draw "Start!" button
}

void Othello::DrawGameScene()
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	// draw wallpaper
	
	// draw gameboard

	// draw disks

	// draw scores and turn

	// draw pause button

	// if "Time Limit" is set, draw left time
}

void Othello::DrawPauseUI()
{
	DrawGameScene();
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	RECT rc;
	GetClientRect(hwnd, &rc);

	D2D1_RECT_F rect = D2D1::RectF(rc.right - rc.left, rc.bottom - rc.top);

	pRenderTarget->FillRectangle(rect, pTranslucentBrush);

	// draw blur/opacity over the background scene

	// draw outline with background

	// draw "Continue", "New Game", "Setting", "Exit" buttons
}

void Othello::DrawFinishUI()
{
	DrawGameScene();
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	RECT rc;
	GetClientRect(hwnd, &rc);

	D2D1_RECT_F rect = D2D1::RectF(rc.right - rc.left, rc.bottom - rc.top);

	pRenderTarget->FillRectangle(rect, pTranslucentBrush);

	// draw winner, scores
	// draw "New Game", "Exit" buttons
}

#pragma endregion

#pragma region Windows

LRESULT CALLBACK Othello::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		Othello* pOthello = (Othello*)pCreate->lpCreateParams;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pOthello);

		return 1;
	}

	Othello* pOthello = (Othello*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (!pOthello)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_SIZE:
	{
		pOthello->OnResize();
		return 0;
	}

	case WM_DISPLAYCHANGE:
	{
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}

	// Don't use OnPaint() at WM_PAINT.
	// Instead, use OnRender() at WinMain().

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 1;
	}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

HRESULT Othello::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateAppResource();
	if (FAILED(hr)) return hr;

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Othello::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"Othello";
	RegisterClassEx(&wcex);

	hwnd = CreateWindow(L"Othello", L"Othello Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, this);
	hr = hwnd ? S_OK : E_FAIL;
	if (FAILED(hr)) return hr;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	return hr;
}

#pragma endregion

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	// try to initialize COM library
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		Othello app;
		if (FAILED(app.Initialize(hInstance))) return 0;

		MSG msg = {};

		// Render(draw) by call OnRender() function.

		// Time
		LARGE_INTEGER nPrevTime;
		LARGE_INTEGER nFrequency;
		QueryPerformanceFrequency(&nFrequency);
		QueryPerformanceCounter(&nPrevTime);

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// Time
				LARGE_INTEGER CurrentTime;
				QueryPerformanceCounter(&CurrentTime);
				float timeDelta = (float)((double)(CurrentTime.QuadPart - nPrevTime.QuadPart) / (double)(nFrequency.QuadPart));
				nPrevTime = CurrentTime;

				GameState::Update(timeDelta);
				if (!app.OnRender(timeDelta))
				{
					break;
				}
			}
		}
	}

	// close COM, and unload all loaded dlls
	CoUninitialize();
	return 0;
}

#pragma region Loading Bitmap (from file)

// Creates a Direct2D bitmap from the specified file name.
HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return hr;

	// Create the initial frame.
	hr = pDecoder->GetFrame(0, &pSource);
	if (FAILED(hr)) return hr;

	// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	hr = pIWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return hr;

	// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
	if (destinationWidth != 0 || destinationHeight != 0)
	{
		UINT originalWidth, originalHeight;
		hr = pSource->GetSize(&originalWidth, &originalHeight);
		if (FAILED(hr)) return hr;

		if (destinationWidth == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
			destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
		}
		else if (destinationHeight == 0)
		{
			FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
			destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
		}

		hr = pIWICFactory->CreateBitmapScaler(&pScaler);
		if (FAILED(hr)) return hr;

		hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
		if (FAILED(hr)) return hr;

		hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}
	else // Don't scale the image.
	{
		hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) return hr;
	}

	// Create a Direct2D bitmap from the WIC bitmap.
	hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	if (FAILED(hr)) return hr;

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}

#pragma endregion