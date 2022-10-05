#pragma once

#include "Settings.h"
#include "Framework.h"

struct Point2D
{
	float x;
	float y;
};

class Sprite
{
protected:
	Point2D m_position{ 0, 0 };
	Point2D m_direction{ 0, 0 };
	float m_speed = 0;
	int m_width = 0, m_height = 0;
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	Sprite() = default;
	virtual ~Sprite();

	virtual void Reset() = 0;
	virtual void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) = 0;

	bool setTexture(ID2D1HwndRenderTarget* m_pRenderTarget, LPCWSTR imageFile);

	void SetPosition(int _x, int _y);
	Point2D GetPosition();

	void SetWidth(int w);
	int GetWidth();

	void SetHeight(int h);
	int GetHeight();
};