#pragma once

//#include "Settings.h"
#include "Framework.h"

#include <vector>
#include <chrono>
#include <random>

#include "Ball.h"
#include "Platform.h"
#include "Block.h"
#include "SafeWall.h"

class Engine : public Framework
{
private:
	ID2D1Factory* m_pDirect2dFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;

	Ball* ball = nullptr;
	Platform* platform = nullptr;
	std::vector<Block*> blocks{ 75, nullptr };
	SafeWall* safeWall = nullptr;

	int mouseXPos = 0;
	int mouseYPos = 0;

	bool playing = false;

	bool win = false;

	bool leftPressed = false;
	bool rightPressed = false;

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	long time = 0;

	bool m_is_transparent_yellow_blocks = false;
	bool m_is_safe_wall_active = false;

	std::random_device m_rd;
	std::mt19937 m_generator;									// generator for distribution
	std::uniform_int_distribution<int> m_distribution_color;	// object for random distribution of m_screen_width

	IDWriteFactory* m_pDWriteFactory = nullptr;
	IDWriteTextFormat* m_pTextFormat = nullptr;
	ID2D1SolidColorBrush* m_pWhiteBrush = nullptr;

	void setBlocksTransparent(bool mode);

	void writeText(const WCHAR text[], float posX, float posY);

	bool isAllBlocksDestroyed();
public:
	Engine();
	~Engine();

	HRESULT InitializeD2D(HWND m_hwnd);
	void ResetAll();
	void Logic(double elapsedTime);
	HRESULT DrawAll();


	void PreInit(int& width, int& height, bool& fullscreen) override;

	bool Init() override;

	void Close() override;

	bool Tick() override;

	void onMouseMove(int x, int y, int xrelative, int yrelative) override;

	void onMouseButtonClick(FRMouseButton button, bool isReleased) override;

	void onKeyPressed(FRKey k) override;

	void onKeyReleased(FRKey k) override;

	const char* GetTitle() override;


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