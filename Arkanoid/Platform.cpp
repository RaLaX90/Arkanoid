#include "Platform.h"

Platform::~Platform()
{
	
}

void Platform::Reset()
{
	m_position.y = RESOLUTION_Y - 20;
	m_position.x = RESOLUTION_X / 2;

	m_speed = 300;
}

void Platform::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	this->Reset();
}

void Platform::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws a rectangle representing the platform
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		m_position.x - PLATFORM_WIDTH / 2 - 10,
		m_position.y - 20,
		m_position.x + PLATFORM_WIDTH / 2 + 10,
		m_position.y + 5
	);

	m_pRenderTarget->DrawBitmap(m_bitmap, rectangle1);  //TO DO (make platform animated)
}

void Platform::Move(FRKey m_direction, float elapsedTime)
{
	if (m_direction == FRKey::LEFT) {
		m_position.x -= m_speed * elapsedTime;
		if (m_position.x < PLATFORM_WIDTH / 2)
		{
			m_position.x = PLATFORM_WIDTH / 2;
		}
	}
	else if (m_direction == FRKey::RIGHT)
	{
		m_position.x += m_speed * elapsedTime;
		if (m_position.x > RESOLUTION_X - PLATFORM_WIDTH / 2)
		{
			m_position.x = RESOLUTION_X - PLATFORM_WIDTH / 2;
		}
	}
}