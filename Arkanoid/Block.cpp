#include "Block.h"

Block::~Block()
{
	SafeRelease(&m_pYellowBrush);
}

void Block::Reset()
{

}

void Block::Reset(colorsSelect color)
{
	m_health = 2;
	m_color = color;
	m_bitmap = nullptr;
}

void Block::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget, colorsSelect color)
{
	this->Reset(color);

	// Creates a green brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pYellowBrush
	);

	m_pYellowBrush->SetOpacity(0.2); // For make yellow blocks transparent || other blocks drawing using a bitmap - not a brush
}

void Block::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (m_bitmap == nullptr && m_health == 2) // loads the character image if not already loaded
	{
		switch (m_color)
		{
		case Block::colorsSelect::Blue: {
			this->setTexture(m_pRenderTarget, L"data/01-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightGreen: {
			this->setTexture(m_pRenderTarget, L"data/03-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Purple: {
			this->setTexture(m_pRenderTarget, L"data/05-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Red: {
			this->setTexture(m_pRenderTarget, L"data/07-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Orange: {
			this->setTexture(m_pRenderTarget, L"data/09-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightBlue: {
			this->setTexture(m_pRenderTarget, L"data/11-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Yellow: {
			this->setTexture(m_pRenderTarget, L"data/13-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Green: {
			this->setTexture(m_pRenderTarget, L"data/15-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Grey: {
			this->setTexture(m_pRenderTarget, L"data/17-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Brown: {
			this->setTexture(m_pRenderTarget, L"data/19-Breakout-Tiles.png");
			break;
		}
		default: {
			break;
		}
		}
	}
	else if (m_bitmap == nullptr && m_health == 1) {
		switch (m_color)
		{
		case Block::colorsSelect::Blue: {
			this->setTexture(m_pRenderTarget, L"data/02-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightGreen: {
			this->setTexture(m_pRenderTarget, L"data/04-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Purple: {
			this->setTexture(m_pRenderTarget, L"data/06-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Red: {
			this->setTexture(m_pRenderTarget, L"data/08-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Orange: {
			this->setTexture(m_pRenderTarget, L"data/10-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::LightBlue: {
			this->setTexture(m_pRenderTarget, L"data/12-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Yellow: {
			this->setTexture(m_pRenderTarget, L"data/14-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Green: {
			this->setTexture(m_pRenderTarget, L"data/16-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Grey: {
			this->setTexture(m_pRenderTarget, L"data/18-Breakout-Tiles.png");
			break;
		}
		case Block::colorsSelect::Brown: {
			this->setTexture(m_pRenderTarget, L"data/20-Breakout-Tiles.png");
			break;
		}
		default: {
			break;
		}
		}
	}

	if (m_health > 0) {
		// Draws a rectangle representing the block
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			m_position.x - BLOCK_WIDTH / 2 + 1,
			m_position.y - BLOCK_HEIGHT / 2 + 1,
			m_position.x + BLOCK_WIDTH / 2 - 1,
			m_position.y + BLOCK_HEIGHT / 2 - 1
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
	return m_health;
}

void Block::Hit() {
	m_health--;
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

//const char* Block::GetTexturePathFromColor(colorsSelect _color)
//{
//	switch (_color)
//	{
//	case Block::colorsSelect::Blue: {
//		return "data/01-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::LightGreen: {
//		return "data/03-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Purple: {
//		return "data/05-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Red: {
//		return "data/07-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Orange: {
//		return "data/09-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::LightBlue: {
//		return "data/11-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Yellow: {
//		return "data/13-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Green: {
//		return "data/15-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Grey: {
//		return "data/17-Breakout-Tiles.png";
//	}
//	case Block::colorsSelect::Brown: {
//		return "data/19-Breakout-Tiles.png";
//	}
//	default: {
//		return nullptr;
//	}
//	}
//}