#pragma once

#include "resource.h"
#include "Settings.h"
#include "Framework.h"
#include "Sprite.h"
#include "Engine.h"

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class MainApp : public Framework
{
private:
	ID2D1Factory* m_pDirect2dFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
	HWND m_hwnd = NULL;

	Engine* engine = nullptr;

	int screenWidth = 0;
	int screenHeight = 0;
	bool isFullScreen = false;

	inline static long m_sPrevMousePosX = 0, m_sPrevMousePosY = 0;
	// The windows procedure.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainApp();
	~MainApp();

	void PreInit(int& m_width, int& m_height, bool& fullscreen) override;

	bool Init() override;

	void Close() override;

	bool Tick() override;

	void onMouseMove(int x, int y, int xrelative, int yrelative) override;

	void onMouseButtonClick(FRMouseButton button, bool isReleased) override;

	void onKeyPressed(FRKey k) override;

	void onKeyReleased(FRKey k) override;

	const char* GetTitle() override;

	// Process and dispatch messages
	void RunMessageLoop();
};