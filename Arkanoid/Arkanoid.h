#pragma once

#include "resource.h"
#include "Settings.h"
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


class MainApp
{
private:
    HWND m_hwnd = NULL;

    Engine* engine = nullptr;

    // The windows procedure.
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    MainApp();
    ~MainApp();

    HRESULT Initialize();

    // Process and dispatch messages
    void RunMessageLoop();
};