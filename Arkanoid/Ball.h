#pragma once

//#include "Sprite.h"
#include "Platform.h"
#include "Block.h"

class Ball : public Sprite
{
public:
	enum class positionState {
		Fixed = 0,
		Free
	};

	enum class collisionTypes {
		None = 0,
		PlatformTouch,
		BlockTouchLeft,
		BlockTouchTop,
		BlockTouchRight,
		BlockTouchBottom,
		BlockDestroy,
		BorderTouchLeft,
		BorderTouchTop,
		BorderTouchRight,
		BorderTouchBottom
	};

	Ball() = default;
	~Ball();

	void Reset() override;
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget, Platform* platform);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	void Move(int mouseX, int mouseY, double elapsedTime);

	positionState GetPositionState();
	void SetPositionState(positionState state);

	collisionTypes CheckPlatformCollision();
	collisionTypes CheckBorderCollision();
	collisionTypes CheckBlockCollision(Block* block);

	void ManageColission(collisionTypes collisionType);

private:
	Platform* platform = nullptr;

	float m_minSpeed = 0, m_maxSpeed = 0;

	positionState m_state;
};