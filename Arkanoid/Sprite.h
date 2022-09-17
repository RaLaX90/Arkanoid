#pragma once

#include "Settings.h"

struct Point2D
{
	float x;
	float y;
};

class Sprite
{
protected:

	ID2D1Bitmap* setBackgroundImage(ID2D1HwndRenderTarget* m_pRenderTarget, LPCWSTR imageFile);

	Point2D position{ 0, 0 };
	Point2D direction{ 0, 0 };
	float m_speed = 0;

	ID2D1Bitmap* m_bitmap = nullptr;

public:
	Sprite();
	~Sprite();

	virtual void Reset() = 0;
	virtual void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget) = 0;
	//virtual void Move(int mouseX, int mouseY, double elapsedTime) = 0;
	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) = 0;

	Point2D GetPosition();
};