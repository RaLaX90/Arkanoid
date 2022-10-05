#include "SafeWall.h"

SafeWall::~SafeWall()
{

}

void SafeWall::Reset()
{

}

void SafeWall::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{

}

void SafeWall::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		0.0F,
		RESOLUTION_Y - 10,
		RESOLUTION_X,
		RESOLUTION_Y
	);

	m_pRenderTarget->DrawBitmap(m_bitmap, rectangle1);
}

void SafeWall::Move(int mouseX, int mouseY, float elapsedTime)
{

}