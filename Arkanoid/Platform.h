#pragma once

#include "Framework.h"
#include "Sprite.h"

class Platform : public Sprite
{
private:
	ID2D1SolidColorBrush* m_pBlueBrush = nullptr;

public:
	Platform();
	~Platform();

	void Reset() override;
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	void Move(FRKey Key, float elapsedTime);
};