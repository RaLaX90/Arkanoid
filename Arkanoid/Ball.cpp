#include "Ball.h"

Ball::~Ball()
{

}

void Ball::Reset()
{
	m_state = positionState::Fixed;

	// Sets an initial ball position and speed.
	m_position.x = platform->GetPosition().x;
	m_position.y = platform->GetPosition().y - 20;

	m_speed = 1; // Here should be 9 on assignment, but to make the game easier here 1)
}

void Ball::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget, Platform* _platform)
{
	platform = _platform;

	this->Reset();

	m_minSpeed = (m_speed * 40) / 100;
	m_maxSpeed = (m_speed * 300) / 100;
}

void Ball::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws the ball using Direct2D
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		m_position.x - 10,
		m_position.y - 10,
		m_position.x + 10,
		m_position.y + 10
	);

	m_pRenderTarget->DrawBitmap(m_bitmap, rectangle1);
}

void Ball::Move(int mouse_position_X, int mouse_position_Y, double elapsed_time)
{
	if (m_state == positionState::Fixed) {
		m_direction.x = mouse_position_X - m_position.x;
		m_direction.y = mouse_position_Y - m_position.y;

		m_position.x = platform->GetPosition().x;
		m_position.y = platform->GetPosition().y - 30; //TO DO (static definition)
	}
	else {
		m_position.x += m_direction.x * (m_speed * elapsed_time);
		m_position.y += m_direction.y * (m_speed * elapsed_time);
	}
}

Ball::positionState Ball::GetPositionState()
{
	return m_state;
}

void Ball::SetPositionState(positionState state)
{
	m_state = state;
}

Ball::collisionTypes Ball::CheckPlatformCollision()
{
	if ((m_position.y > RESOLUTION_Y - 50 /*&& m_direction.y > 0*/) && (m_position.x > (platform->GetPosition().x - PLATFORM_WIDTH / 2 - 10) && m_position.x < (platform->GetPosition().x + PLATFORM_WIDTH / 2 + 10))) {
		return collisionTypes::PlatformTouch;
	}

	return collisionTypes::None;
}

Ball::collisionTypes Ball::CheckBorderCollision()
{
	if (m_position.x > RESOLUTION_X - BALL_RADIUS) {
		return collisionTypes::BorderTouchRight;
	}
	else if (m_position.x < BALL_RADIUS) {
		return collisionTypes::BorderTouchLeft;
	}
	else if (m_position.y < BALL_RADIUS) {
		return collisionTypes::BorderTouchTop;
	}
	else if (m_position.y > RESOLUTION_Y - BALL_RADIUS) {
		return collisionTypes::BorderTouchBottom;
	}

	return collisionTypes::None;
}

Ball::collisionTypes Ball::CheckBlockCollision(Block* block) //TO DO (parameter can be improved)
{
	if ((*block).GetHealth() > 0)
	{
		float blockTop = (*block).GetPosition().y - BLOCK_HEIGHT / 2;
		float blockBottom = (*block).GetPosition().y + BLOCK_HEIGHT / 2;
		float blockLeft = (*block).GetPosition().x - BLOCK_WIDTH / 2;
		float blockRight = (*block).GetPosition().x + BLOCK_WIDTH / 2;

		float ballTop = m_position.y - BALL_RADIUS;
		float ballBottom = m_position.y + BALL_RADIUS;
		float ballLeft = m_position.x - BALL_RADIUS;
		float ballRight = m_position.x + BALL_RADIUS;

		// If it's a collision
		if (ballTop < blockBottom && ballBottom > blockTop && ballRight > blockLeft && ballLeft < blockRight)
		{
			float distanceX1 = abs(ballRight - blockLeft);
			float distanceX2 = abs(ballLeft - blockRight);
			float distanceY1 = abs(ballTop - blockBottom);
			float distanceY2 = abs(ballBottom - blockTop);

			// From bottom
			if (m_direction.y < 0 && distanceY1 < distanceY2 && distanceY1 < distanceX1 && distanceY1 < distanceX2)
			{
				return collisionTypes::BlockTouchBottom;
			}
			// From top
			if (m_direction.y > 0 && distanceY2 < distanceY1 && distanceY2 < distanceX1 && distanceY2 < distanceX2)
			{
				return collisionTypes::BlockTouchTop;
			}
			// From left
			if (m_direction.x > 0 && distanceX1 < distanceX2 && distanceX1 < distanceY1 && distanceX1 < distanceY2)
			{
				return collisionTypes::BlockTouchLeft;
			}
			// From right
			if (m_direction.x < 0 && distanceX2 < distanceX1 && distanceX2 < distanceY1 && distanceX2 < distanceY2)
			{
				return collisionTypes::BlockTouchRight;
			}
		}
	}
	/*else if (block.GetHealth() == 0) { //TO DO (remove finding "dead" block)
		auto a = 0;
	}*/

	return collisionTypes::None;
}

void Ball::ManageColission(collisionTypes collisionType)
{
	switch (collisionType)
	{
	case Ball::collisionTypes::PlatformTouch: {
		float platformHitPos = (m_position.x - platform->GetPosition().x) / (PLATFORM_WIDTH / 2);
		m_direction.x = platformHitPos * RECOIL_X_MAX;
		m_direction.y = -m_direction.y;

		float increasedSpeed = (m_speed * 15) / 100;

		if ((m_speed + increasedSpeed) > m_maxSpeed) {
			m_speed = m_maxSpeed;
		}
		else {
			m_speed += increasedSpeed;
		}
		break;
	}
	case Ball::collisionTypes::BlockTouchLeft: {
		m_direction.x = -abs(m_direction.x);
		break;
	}
	case Ball::collisionTypes::BlockTouchTop: {
		m_direction.y = -abs(m_direction.y);
		break;
	}
	case Ball::collisionTypes::BlockTouchRight: {
		m_direction.x = abs(m_direction.x);
		break;
	}
	case Ball::collisionTypes::BlockTouchBottom: {
		m_direction.y = abs(m_direction.y);
		break;
	}
	case Ball::collisionTypes::BlockDestroy: {
		float reducedSpeed = (m_speed * 10) / 100;

		if ((m_speed - reducedSpeed) < m_minSpeed) {
			m_speed = m_minSpeed;
		}
		else {
			m_speed -= reducedSpeed;
		}
		break;
	}
	case Ball::collisionTypes::BorderTouchLeft: {
		m_direction.x = abs(m_direction.x);
		break;
	}
	case Ball::collisionTypes::BorderTouchTop: {
		m_direction.y = abs(m_direction.y);
		break;
	}
	case Ball::collisionTypes::BorderTouchRight: {
		m_direction.x = -abs(m_direction.x);
		break;
	}
	//case Ball::collisionTypes::BorderTouchBottom: {
	//	
	//	break;
	//}
	case Ball::collisionTypes::None:
	default: {

		break;
	}
	}
}