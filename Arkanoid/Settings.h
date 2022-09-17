// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <windowsx.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>


template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

#define BLOCK_WIDTH 40
#define BLOCK_HEIGHT 20

#define PLATFORM_HEIGHT 30
#define PLATFORM_WIDTH 100

#define BALL_RADIUS 10

#define RECOIL_X_MAX 300