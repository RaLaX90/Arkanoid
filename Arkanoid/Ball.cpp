#include "Ball.h"

Ball::Ball(Platform* platform) : m_platform(platform)
{
	this->Reset();
}

Ball::~Ball()
{
	SafeRelease(&m_pRedBrush);
}

void Ball::Reset()
{
	m_state = positionState::Fixed;

	// Sets an initial ball position and speed.
	position.x = m_platform->GetPosition().x;
	position.y = m_platform->GetPosition().y - 20;

	m_speed = 1; // Here should be 9 on assignment, but to make the game easier here 1)
}

void Ball::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Initializes Direct2D red brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pRedBrush
	);

	m_minSpeed = (m_speed * 40) / 100;
	m_maxSpeed = (m_speed * 300) / 100;
}

void Ball::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws the ball using Direct2D
	D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
		D2D1::Point2F(position.x, position.y),
		BALL_RADIUS, BALL_RADIUS
	);

	m_pRenderTarget->FillEllipse(&ellipseBall, m_pRedBrush);
}

void Ball::Move(int mouse_position_X, int mouse_position_Y, double elapsed_time)
{
	if (m_state == positionState::Fixed) {
		direction.x = mouse_position_X - position.x;
		direction.y = mouse_position_Y - position.y;

		position.x = m_platform->GetPosition().x;
		position.y = m_platform->GetPosition().y - 30; //TO DO (static definition)
	}
	else {
		position.x += direction.x * (m_speed * elapsed_time);
		position.y += direction.y * (m_speed * elapsed_time);
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
	if ((position.y > RESOLUTION_Y - 50 /*&& direction.y > 0*/) && (position.x > (m_platform->GetPosition().x - PLATFORM_WIDTH / 2 - 10) && position.x < (m_platform->GetPosition().x + PLATFORM_WIDTH / 2 + 10))) {
		return collisionTypes::PlatformTouch;
	}

	return collisionTypes::None;
}

Ball::collisionTypes Ball::CheckBorderCollision()
{
	if (position.x > RESOLUTION_X - BALL_RADIUS) {
		return collisionTypes::BorderTouchRight;
	}
	else if (position.x < BALL_RADIUS) {
		return collisionTypes::BorderTouchLeft;
	}
	else if (position.y < BALL_RADIUS) {
		return collisionTypes::BorderTouchTop;
	}
	else if (position.y > RESOLUTION_Y - BALL_RADIUS) {
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

		float ballTop = position.y - BALL_RADIUS;
		float ballBottom = position.y + BALL_RADIUS;
		float ballLeft = position.x - BALL_RADIUS;
		float ballRight = position.x + BALL_RADIUS;

		// If it's a collision
		if (ballTop < blockBottom && ballBottom > blockTop && ballRight > blockLeft && ballLeft < blockRight)
		{
			float distanceX1 = abs(ballRight - blockLeft);
			float distanceX2 = abs(ballLeft - blockRight);
			float distanceY1 = abs(ballTop - blockBottom);
			float distanceY2 = abs(ballBottom - blockTop);

			// From bottom
			if (direction.y < 0 && distanceY1 < distanceY2 && distanceY1 < distanceX1 && distanceY1 < distanceX2)
			{
				return collisionTypes::BlockTouchBottom;
			}
			// From top
			if (direction.y > 0 && distanceY2 < distanceY1 && distanceY2 < distanceX1 && distanceY2 < distanceX2)
			{
				return collisionTypes::BlockTouchTop;
			}
			// From left
			if (direction.x > 0 && distanceX1 < distanceX2 && distanceX1 < distanceY1 && distanceX1 < distanceY2)
			{
				return collisionTypes::BlockTouchLeft;
			}
			// From right
			if (direction.x < 0 && distanceX2 < distanceX1 && distanceX2 < distanceY1 && distanceX2 < distanceY2)
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
		float platformHitPos = (position.x - m_platform->GetPosition().x) / (PLATFORM_WIDTH / 2);
		direction.x = platformHitPos * RECOIL_X_MAX;
		direction.y = -direction.y;

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
		direction.x = -abs(direction.x);
		break;
	}
	case Ball::collisionTypes::BlockTouchTop: {
		direction.y = -abs(direction.y);
		break;
	}
	case Ball::collisionTypes::BlockTouchRight: {
		direction.x = abs(direction.x);
		break;
	}
	case Ball::collisionTypes::BlockTouchBottom: {
		direction.y = abs(direction.y);
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
		direction.x = abs(direction.x);
		break;
	}
	case Ball::collisionTypes::BorderTouchTop: {
		direction.y = abs(direction.y);
		break;
	}
	case Ball::collisionTypes::BorderTouchRight: {
		direction.x = -abs(direction.x);
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