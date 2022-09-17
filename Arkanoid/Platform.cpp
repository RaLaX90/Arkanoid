#include "Platform.h"

Platform::Platform()
{
	this->Reset();
}

Platform::~Platform()
{
	SafeRelease(&m_pBlueBrush);
}

void Platform::Reset()
{
	position.y = RESOLUTION_Y - 20;
	position.x = RESOLUTION_X / 2;

	m_speed = 300;
}

void Platform::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a blue brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&m_pBlueBrush
	);

	m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/50-Breakout-Tiles.png");
}

void Platform::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws a rectangle representing the platform
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		position.x - PLATFORM_WIDTH / 2 - 10,
		position.y - 20,
		position.x + PLATFORM_WIDTH / 2 + 10,
		position.y + 5
	);

	m_pRenderTarget->DrawBitmap(m_bitmap, rectangle1);  //TO DO (make platform animated)
}

void Platform::Move(FRKey direction, float elapsedTime)
{
	if (direction == FRKey::LEFT) {
		position.x -= m_speed * elapsedTime;
		if (position.x < PLATFORM_WIDTH / 2)
		{
			position.x = PLATFORM_WIDTH / 2;
		}
	}
	else if (direction == FRKey::RIGHT)
	{
		position.x += m_speed * elapsedTime;
		if (position.x > RESOLUTION_X - PLATFORM_WIDTH / 2)
		{
			position.x = RESOLUTION_X - PLATFORM_WIDTH / 2;
		}
	}
}