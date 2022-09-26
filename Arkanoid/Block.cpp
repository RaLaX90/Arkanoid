#include "Block.h"

Block::Block(float xPos, float yPos, colorsSelect color)
{
	position.x = xPos;
	position.y = yPos;

	this->Reset(color);
}

Block::~Block()
{
	SafeRelease(&m_pYellowBrush);
}

void Block::Reset()
{

}

void Block::Reset(colorsSelect color)
{
	health = 2;
	m_color = color;
	m_bitmap = nullptr;
}

void Block::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a green brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pYellowBrush
	);

	m_pYellowBrush->SetOpacity(0.2); // For make yellow blocks transparent || other blocks drawing using a bitmap - not a brush
}

void Block::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (m_bitmap == nullptr && health == 2) // loads the character image if not already loaded
	{
		switch (m_color)
		{
		case Block::colorsSelect::Blue: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/01-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightGreen: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/03-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Purple: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/05-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Red: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/07-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Orange: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/09-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightBlue: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/11-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Yellow: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/13-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Green: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/15-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Grey: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/17-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Brown: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/19-Breakout-Tiles.png");
			break;
		}
		default: {
			break;
		}
		}
	}
	else if (m_bitmap == nullptr && health == 1) {
		switch (m_color)
		{
		case Block::colorsSelect::Blue: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/02-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightGreen: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/04-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Purple: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/06-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Red: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/08-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Orange: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/10-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightBlue: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/12-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Yellow: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/14-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Green: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/16-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Grey: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/18-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Brown: {
			m_bitmap = setBackgroundImage(m_pRenderTarget, L"data/20-Breakout-Tiles.png");
			break;
		}
		default: {
			break;
		}
		}
	}

	if (health > 0) {
		// Draws a rectangle representing the block
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			position.x - BLOCK_WIDTH / 2 + 1,
			position.y - BLOCK_HEIGHT / 2 + 1,
			position.x + BLOCK_WIDTH / 2 - 1,
			position.y + BLOCK_HEIGHT / 2 - 1
		);

		if (!m_transparentMode) {
			m_pRenderTarget->DrawBitmap(m_bitmap, rectangle1);
		}
		else {
			m_pRenderTarget->FillRectangle(&rectangle1, m_pYellowBrush);
		}
	}
}

uint8_t Block::GetHealth() {
	return health;
}

void Block::Hit() {
	health--;
	m_bitmap = nullptr;
}

Block::colorsSelect Block::GetColor() {
	return m_color;
}

void Block::SetTransparentMode(bool mode) {
	m_transparentMode = mode;
}

bool Block::GetTransparentMode() {
	return m_transparentMode;
}