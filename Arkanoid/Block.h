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

	Block() = default;
	~Block();

	void Reset() override;
	void Reset(colorsSelect color);
	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget, colorsSelect color);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	uint8_t GetHealth();
	void Hit();
	colorsSelect GetColor();

	void SetTransparentMode(bool mode);
	bool GetTransparentMode();

	//static const char* GetTexturePathFromColor(colorsSelect _color);
private:
	colorsSelect m_color;

	ID2D1SolidColorBrush* m_pYellowBrush = nullptr;

	uint8_t m_health = 0;

	bool m_transparentMode = false;
};