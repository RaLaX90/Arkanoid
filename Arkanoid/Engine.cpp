#include "Engine.h"

Engine::~Engine()
{
	for (auto& block : blocks) {
		destroySprite(block);
	}
	blocks.clear();
	blocks.shrink_to_fit();

	//destroySprite(safeWall);
	destroySprite(ball);
	destroySprite(platform);
}

void Engine::InitializeD2D(ID2D1HwndRenderTarget* renderTarget)
{
	m_pRenderTarget = renderTarget;

	platform = dynamic_cast<Platform*>(createSprite<Platform>("data/50-Breakout-Tiles.png"));
	platform->Initialize(m_pRenderTarget);

	ball = dynamic_cast<Ball*>(createSprite<Ball>("data/58-Breakout-Tiles.png"));
	ball->Initialize(m_pRenderTarget, platform);

	//safeWall = dynamic_cast<SafeWall*>(createSprite<SafeWall>("data/61-Breakout-Tiles.png")); 
	//safeWall->Initialize(m_pRenderTarget);

	m_generator = std::mt19937(m_rd());
	m_distribution_color = std::uniform_int_distribution<int>(0, 9);

	// Intitilize blocks
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 15; j++) //TO DO (can be upgrade to full dynamic calculation)
		{
			float posX = RESOLUTION_X / 2 + (j - 7) * BLOCK_WIDTH;
			float posY = 100 + i * BLOCK_HEIGHT;
			blocks[i * 15 + j] = new Block();
			//blocks[i * 15 + j] = dynamic_cast<Block*>(createSprite<Block>("data/50-Breakout-Tiles.png"));
			blocks[i * 15 + j]->SetPosition(posX, posY);
			blocks[i * 15 + j]->Initialize(m_pRenderTarget, (Block::colorsSelect)m_distribution_color(m_generator));
		}
	}

	m_isPlaying = true;

	std::thread([&]
		{
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(30));
				m_isTransparentYellowBlocks = true;
				setBlocksTransparent(true);

				std::this_thread::sleep_for(std::chrono::seconds(3));
				m_isTransparentYellowBlocks = false;
				setBlocksTransparent(false);
			}
		}).detach();

		timeAfterInitialization = std::chrono::steady_clock::now();
}

void Engine::ResetAll()
{
	// This method reset the game, given that the game was won or lost

	if (ball->GetPositionState() == Ball::positionState::Fixed) {
		ball->SetPositionState(Ball::positionState::Free);
	}

	if (!m_isPlaying)
	{
		platform->Reset();
		ball->Reset();
		for (const auto& block : blocks) {
			block->Reset((Block::colorsSelect)m_distribution_color(m_generator));
		}

		m_isPlaying = true;
		m_isWin = false;
	}
}

void Engine::Logic(double elapsedTime)
{
	// This is the logic part of the engine. It receives the elapsed time from the app class, in seconds.
	// It uses this value for a smooth and consistent movement, regardless of the CPU or graphics speed
	if (m_isPlaying)
	{
		if (platformMovementSide == FRKey::LEFT || platformMovementSide == FRKey::RIGHT) {
			platform->Move(platformMovementSide, elapsedTime);
		}

		// Moves the ball forward
		ball->Move(m_mouseXPos, m_mouseYPos, elapsedTime);

		if (ball->GetPositionState() != Ball::positionState::Fixed) {

			Ball::collisionTypes colission;

			{ //ball vs border
				colission = ball->CheckBorderCollision();
				ball->ManageColission(colission);

				if (colission == Ball::collisionTypes::BorderTouchBottom) {
					m_isPlaying = false;
				}
			}

			{ //ball vs platform
				colission = ball->CheckPlatformCollision();
				ball->ManageColission(colission);

				//m_isWin logic
				if (isAllBlocksDestroyed() && colission == Ball::collisionTypes::PlatformTouch) {
					m_isPlaying = false;
					m_isWin = true;
				}
			}

			{ //ball vs blocks
				for (const auto& block : blocks) {
					//  If not a time - drawing all, if not - drawing only needed blocks
					if (!m_isTransparentYellowBlocks || (m_isTransparentYellowBlocks && !block->GetTransparentMode())) {

						colission = ball->CheckBlockCollision(block);
						ball->ManageColission(colission);

						if (colission != Ball::collisionTypes::None) { // If colission was - "hit" blocks
							block->Hit();

							if (block->GetHealth() <= 0) { // If after hits m_health is 0 - manage it
								ball->ManageColission(Ball::collisionTypes::BlockDestroy);
							}
						}
					}
				}
			}
		}
	}
}

HRESULT Engine::DrawAll()
{
	// This is the drawing method of the engine.
	// It simply draws all the elements in the game using Direct2D
	HRESULT hr;

	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	drawSprite(platform, platform->GetPosition().x, platform->GetPosition().y);

	drawSprite(ball, ball->GetPosition().x, ball->GetPosition().y);

	//drawSprite(safeWall, safeWall->GetPosition().x, safeWall->GetPosition().y); //TO DO (Safe wall drawing (ready without logic))

	for (const auto& block : blocks) {
		drawSprite(block, block->GetPosition().x, block->GetPosition().y);
		//block->Draw(m_pRenderTarget);
	}

	if (!m_isPlaying) {
		if (!m_isWin) {
			writeText(L"Lose( lucky next time (Press LMB for restart)", RESOLUTION_X, RESOLUTION_Y);
		}
		else {
			writeText(L"Winner Winner Chicken Dinner! (Press LMB for restart)", RESOLUTION_X, RESOLUTION_Y);
		}
	}

	hr = m_pRenderTarget->EndDraw();

	return hr;
}

void Engine::SetMousePosition(int mouse_X, int mouse_Y)
{
	m_mouseXPos = mouse_X;
	m_mouseYPos = mouse_Y;
}

int Engine::GetMousePositionX()
{
	return m_mouseXPos;
}

int Engine::GetMousePositionY()
{
	return m_mouseYPos;
}

void Engine::SetSideButtonPressed(FRKey side)
{
	platformMovementSide = side;
}

FRKey Engine::GetSideButtonPressed()
{
	return platformMovementSide;
}

void Engine::setBlocksTransparent(bool mode) {
	for (const auto& block : blocks) {
		if (block->GetColor() == Block::colorsSelect::Yellow) {
			block->SetTransparentMode(mode);
		}
	}
}

void Engine::writeText(const WCHAR text[], float posX, float posY)
{
	IDWriteFactory* m_pDWriteFactory = nullptr;

	// Initialize text writing factory and format
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
	);

	IDWriteTextFormat* m_pTextFormat = nullptr;

	m_pDWriteFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50,
		L"", //locale
		&m_pTextFormat
	);

	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	ID2D1SolidColorBrush* m_pWhiteBrush = nullptr;

	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	);

	m_pRenderTarget->DrawText(
		text,
		((std::wstring)text).length(),  //TO DO (static definition)
		m_pTextFormat,
		D2D1::RectF(0, 0, posX, posY),
		m_pWhiteBrush
	);

	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pDWriteFactory);
}

bool Engine::isAllBlocksDestroyed()
{
	bool isAllBlocksDestroyed = true;
	for (const auto& block : blocks) {
		if (block->GetHealth()) {
			isAllBlocksDestroyed = false;
			break;
		}
	}

	return isAllBlocksDestroyed;
}

template<typename T>
inline Sprite* Engine::createSprite(const char* path)
{
	Sprite* obj = new T();
	wchar_t* wString = new wchar_t[50];
	MultiByteToWideChar(CP_ACP, 0, path, -1, wString, 50);
	bool isSettingsTextureSuccessed = obj->setTexture(m_pRenderTarget, wString);
	return obj;
}

void Engine::drawSprite(Sprite* s, int x, int y)
{
	//s->SetPosition(x, y);
	s->Draw(m_pRenderTarget);
}

void Engine::getSpriteSize(Sprite* s, int& w, int& h)
{
	w = s->GetWidth();
	h = s->GetHeight();
}

void Engine::setSpriteSize(Sprite* s, int w, int h)
{
	s->SetWidth(w);
	s->SetHeight(h);
}

void Engine::destroySprite(Sprite* s)
{
	delete s;
}

void Engine::drawTestBackground()
{
}

void Engine::getScreenSize(int& w, int& h)
{
	w = RESOLUTION_X;
	h = RESOLUTION_Y;
}

unsigned int Engine::getTickCount()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now() - timeAfterInitialization
		).count();
}

void Engine::showCursor(bool bShow) {
	ShowCursor(bShow);
}