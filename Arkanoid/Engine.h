#pragma once

//#include "Settings.h"
#include "Framework.h"

#include <vector>
#include <chrono>
#include <random>
#include <thread>

#include "Ball.h"
//#include "Platform.h"
//#include "Block.h"
//#include "SafeWall.h"

class Engine
{
private:
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

	Ball* ball = nullptr;
	Platform* platform = nullptr;
	std::vector<Block*> blocks{ 75, nullptr };
	//SafeWall* safeWall = nullptr;

	int m_mouseXPos = 0;
	int m_mouseYPos = 0;

	bool m_isPlaying = false;

	bool m_isWin = false;

	std::chrono::steady_clock::time_point timeAfterInitialization;

	bool m_isTransparentYellowBlocks = false;
	bool m_isSafeWallActive = false;

	std::random_device m_rd;
	std::mt19937 m_generator;									// generator for distribution
	std::uniform_int_distribution<int> m_distribution_color;	// object for random distribution of m_screen_width

	void setBlocksTransparent(bool mode);

	void writeText(const WCHAR text[], float posX, float posY);

	bool isAllBlocksDestroyed();

public:
	Engine() = default;
	~Engine();

	FRKey platformMovementSide = FRKey::COUNT;

	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void ResetAll();
	void Logic(double elapsedTime);
	HRESULT DrawAll();

	void SetMousePosition(int mouse_X, int mouse_Y);
	int GetMousePositionX();
	int GetMousePositionY();

	void SetSideButtonPressed(FRKey side);
	FRKey GetSideButtonPressed();

	template <typename T>
	Sprite* createSprite(const char* path);
	void drawSprite(Sprite*, int x, int y);

	void getSpriteSize(Sprite* s, int& w, int& h);
	void setSpriteSize(Sprite* s, int w, int h);
	void destroySprite(Sprite* s);

	void drawTestBackground();

	void getScreenSize(int& w, int& h);

	unsigned int getTickCount();

	void showCursor(bool bShow);
};