// Arkanoid.cpp : Defines the entry point for the application.
//

#include "Arkanoid.h"

//#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		MainApp app;

		if (SUCCEEDED(app.Init()))
		{
			app.RunMessageLoop();
		}

		CoUninitialize();
	}

	return 0;
}

MainApp::MainApp()
{
	engine = new Engine();

	screenWidth = RESOLUTION_X;
	screenHeight = RESOLUTION_Y;
}

MainApp::~MainApp()
{
	delete engine;

	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDirect2dFactory);
}

void MainApp::PreInit(int& m_width, int& m_height, bool& fullscreen)
{
	m_width = screenWidth;
	m_height = screenHeight;
	fullscreen = isFullScreen;
}

bool MainApp::Init()
{
	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"D2DMainApp";

	ATOM x = RegisterClassEx(&wcex);

	// Create the window.
	m_hwnd = CreateWindowEx(
		NULL,
		L"D2DMainApp",
		(LPCWSTR)GetTitle(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		RESOLUTION_X,
		RESOLUTION_Y,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	// Initializes Direct2D, to draw with
	D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&m_pRenderTarget
	);

	HRESULT hr = m_hwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		engine->InitializeD2D(m_pRenderTarget);

		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);
	}

	return SUCCEEDED(hr);
}

void MainApp::Close()
{

}

bool MainApp::Tick()
{
	return false;
}

void MainApp::onMouseMove(int x, int y, int xrelative, int yrelative)
{
	engine->SetMousePosition(x, y);
}

void MainApp::onMouseButtonClick(FRMouseButton button, bool isReleased)
{
	if (button == FRMouseButton::LEFT) {
		engine->ResetAll();
	}
}

void MainApp::onKeyPressed(FRKey k)
{
	if (k == FRKey::LEFT || k == FRKey::RIGHT) {
		engine->SetSideButtonPressed(k);
	}
}

void MainApp::onKeyReleased(FRKey k)
{
	if (((k == FRKey::LEFT) && (engine->GetSideButtonPressed() == FRKey::LEFT)) ||
		((k == FRKey::RIGHT) && (engine->GetSideButtonPressed() == FRKey::RIGHT))) {
		engine->SetSideButtonPressed(FRKey::COUNT);
	}
}

const char* MainApp::GetTitle()
{
	return "Arkanoid";
}

void MainApp::RunMessageLoop()
{
	MSG msg{};

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	int frames = 0;
	double framesTime = 0;

	boolean running = true;
	double elapsed_secs;
	while (running)
	{
		end = std::chrono::steady_clock::now();
		elapsed_secs = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
		begin = end;

		// Display FPS
		framesTime += elapsed_secs;
		frames++;
		if (framesTime > 1) {
			WCHAR fpsText[32];
			swprintf(fpsText, 32, L"Game: %d FPS", frames);
			SetWindowText(m_hwnd, fpsText);
			frames = 0;
			framesTime = 0;
		}

		// Messages and user input
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT || (msg.message == WM_CHAR && msg.wParam == VK_ESCAPE)) {
				running = false;
			}
		}

		// Game logic
		engine->Logic(elapsed_secs);

		// Drawing
		engine->DrawAll();
	}
}

LRESULT CALLBACK MainApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		MainApp* pMainApp = (MainApp*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(pMainApp)
		);

		result = 1;
	}
	else
	{
		MainApp* pMainApp = reinterpret_cast<MainApp*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pMainApp)
		{
			switch (message)
			{
			case WM_MOUSEMOVE:
			{
				POINT lpPoint;
				GetCursorPos(&lpPoint);
				ScreenToClient(hwnd, &lpPoint);
				pMainApp->onMouseMove(lpPoint.x, lpPoint.y, lpPoint.x - m_sPrevMousePosX, lpPoint.y - m_sPrevMousePosY);

				m_sPrevMousePosX = lpPoint.x;
				m_sPrevMousePosY = lpPoint.y;

				result = 0;
				wasHandled = true;
				break;
			}
			case WM_LBUTTONUP:
			{
				pMainApp->onMouseButtonClick(FRMouseButton::LEFT, true);
				result = 0;
				wasHandled = true;
				break;
			}
			case WM_KEYDOWN:
			{
				if (wParam == VK_LEFT) {
					pMainApp->onKeyPressed(FRKey::LEFT);
				}
				else if (wParam == VK_RIGHT) {
					pMainApp->onKeyPressed(FRKey::RIGHT);
				}
				result = 0;
				wasHandled = true;
				break;
			}
			case WM_KEYUP:
			{
				if (wParam == VK_LEFT) {
					pMainApp->onKeyReleased(FRKey::LEFT);
				}
				else if (wParam == VK_RIGHT) {
					pMainApp->onKeyReleased(FRKey::RIGHT);
				}
				result = 0;
				wasHandled = true;
				break;
			}
			//case WM_SETCURSOR: {
			//	SetCursor(NULL);
			//	engine->showCursor(false);
			//	result = 0;
			//	wasHandled = true;
			//	break;
			//}
			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
				result = 0;
				wasHandled = true;
				break;
			}
			case WM_CLOSE: {
				DestroyWindow(hwnd);
				break;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				result = 1;
				wasHandled = true;
				break;
			}
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}