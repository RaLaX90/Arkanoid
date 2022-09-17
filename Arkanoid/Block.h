#pragma once

#include "Sprite.h"

class Block : public Sprite
{
public:
	enum class colorsSelect
	{
		Blue = 0,
		LightGreen,
		Purple,
		Red,
		Orange,
		LightBlue,
		Yellow,
		Green,
		Grey,
		Brown
	};

	Block(float xPos, float yPos, colorsSelect color);
	~Block();

	void Reset() override;
	void Reset(colorsSelect color);
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	uint8_t GetHealth();
	void Hit();
	colorsSelect GetColor();

	void SetTransparentMode(bool mode);
	bool GetTransparentMode();
private:
	colorsSelect m_color;

	ID2D1SolidColorBrush* m_pYellowBrush = nullptr;

	uint8_t health = 0;

	bool m_transparentMode = false;
};