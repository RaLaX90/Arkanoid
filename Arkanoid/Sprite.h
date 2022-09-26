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





public:

	Point2D position{ 0, 0 };
	Point2D direction{ 0, 0 };
	float m_speed = 0;
	int width, height;
	ID2D1Bitmap* m_bitmap = nullptr;

	static ID2D1Bitmap* setBackgroundImage(ID2D1HwndRenderTarget* m_pRenderTarget, LPCWSTR imageFile);

	Sprite();
	~Sprite();

	virtual void Reset();
	virtual void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	//virtual void Move(int mouseX, int mouseY, double elapsedTime) = 0;
	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D GetPosition();

	void SetWidth(int w);
	int GetWidth();

	void SetHeight(int h);
	int GetHeight();
};