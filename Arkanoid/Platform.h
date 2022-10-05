#pragma once

#include "Sprite.h"

class Platform : public Sprite
{
public:
	Platform() = default;
	~Platform();

	void Reset() override;
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	void Move(FRKey Key, float elapsedTime);
};