#include "Engine.h"

Engine::Engine()
{
	// Initializes everything in the game: ball, platform, positions

	platform = new Platform();
	ball = new Ball(platform);

	safeWall = new SafeWall();

	m_generator = std::mt19937(m_rd());
	m_distribution_color = std::uniform_int_distribution<int>(0, 9);

	// Intitilize blocks
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 15; j++) //TO DO (can be upgrade to full dynamic calculation)
		{
			float posX = RESOLUTION_X / 2 + (j - 7) * BLOCK_WIDTH;
			float posY = 100 + i * BLOCK_HEIGHT;
			blocks[i * 15 + j] = new Block(posX, posY, (Block::colorsSelect)m_distribution_color(m_generator));
		}
	}

	playing = true;

	begin = std::chrono::steady_clock::now();
}

Engine::~Engine()
{
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pDWriteFactory);

	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDirect2dFactory);


	for (auto& block : blocks) {
		delete block;
	}
	blocks.clear();
	blocks.shrink_to_fit();

	delete safeWall;
	delete ball;
	delete platform;
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
	// Initializes Direct2D, to draw with
	D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&m_pRenderTarget
	);

	ball->Initialize(m_pRenderTarget);

	platform->Initialize(m_pRenderTarget);

	safeWall->Initialize(m_pRenderTarget);

	for (const auto& block : blocks) {
		block->Initialize(m_pRenderTarget);
	}

	return S_OK;
}

void Engine::ResetAll()
{
	// This method reset the game, given that the game was won or lost

	if (ball->GetPositionState() == Ball::positionState::Fixed) {
		ball->SetPositionState(Ball::positionState::Free);
	}

	if (!playing)
	{
		platform->Reset();
		ball->Reset();
		for (const auto& block : blocks) {
			block->Reset((Block::colorsSelect)m_distribution_color(m_generator));
		}

		playing = true;
	}
}

void Engine::Logic(double elapsedTime)
{
	// This is the logic part of the engine. It receives the elapsed time from the app class, in seconds.
	// It uses this value for a smooth and consistent movement, regardless of the CPU or graphics speed
	if (playing)
	{
		if (leftPressed) {
			platform->Move(FRKey::LEFT, elapsedTime);
		}
		if (rightPressed) {
			platform->Move(FRKey::RIGHT, elapsedTime);
		}

		// Moves the ball forward
		ball->Move(mouseXPos, mouseYPos, elapsedTime);

		end = std::chrono::steady_clock::now();
		time = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

		if (!m_is_transparent_yellow_blocks && time >= 10) {
			m_is_transparent_yellow_blocks = true;
			setBlocksTransparent(true);
			begin = end;
		}
		else if (m_is_transparent_yellow_blocks && time >= 3) {
			m_is_transparent_yellow_blocks = false;
			setBlocksTransparent(false);
			begin = end;
		}

		if (ball->GetPositionState() != Ball::positionState::Fixed) {

			Ball::collisionTypes colission;

			{ //ball vs border
				colission = ball->CheckBorderCollision();
				ball->ManageColission(colission);

				if (colission == Ball::collisionTypes::BorderTouchBottom) {
					playing = false;
				}
			}

			{ //ball vs platform
				colission = ball->CheckPlatformCollision();
				ball->ManageColission(colission);

				//win logic
				if (isAllBlocksDestroyed() && colission == Ball::collisionTypes::PlatformTouch) {
					win = true;
				}
			}

			{ //ball vs blocks
				for (const auto& block : blocks) {
					//  If not a time - drawing all, if not - drawing only needed blocks
					if (!m_is_transparent_yellow_blocks || (m_is_transparent_yellow_blocks && !block->GetTransparentMode())) {

						colission = ball->CheckBlockCollision(block);
						ball->ManageColission(colission);

						if (colission != Ball::collisionTypes::None) { // If colission was - "hit" blocks
							block->Hit();

							if (block->GetHealth() <= 0) { // If after hits health is 0 - manage it
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

	platform->Draw(m_pRenderTarget);

	ball->Draw(m_pRenderTarget);

	//safeWall->Draw(m_pRenderTarget);  //TO DO (Safe wall drawing (ready without logic))

	for (const auto& block : blocks) {
		block->Draw(m_pRenderTarget);
	}

	if (!playing) {
		writeText(L"You lose( Press LMB for restart", RESOLUTION_X, RESOLUTION_Y);
	}

	hr = m_pRenderTarget->EndDraw();

	return S_OK;
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
	// Initialize text writing factory and format
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
	);

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

	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	);

	m_pRenderTarget->DrawText(
		text,
		31,  //TO DO (static definition)
		m_pTextFormat,
		D2D1::RectF(0, 0, posX, posY),
		m_pWhiteBrush
	);
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


void Engine::PreInit(int& width, int& height, bool& fullscreen)
{
}

bool Engine::Init()
{
	return false;
}

void Engine::Close()
{
}

bool Engine::Tick()
{
	return false;
}

void Engine::onMouseMove(int x, int y, int xrelative, int yrelative)
{
	mouseXPos = x;
	mouseYPos = y;
}

void Engine::onMouseButtonClick(FRMouseButton button, bool isReleased)
{
}

void Engine::onKeyPressed(FRKey k)
{
	if (k == FRKey::LEFT) {
		leftPressed = true;
	}
	if (k == FRKey::RIGHT) {
		rightPressed = true;
	}
}

void Engine::onKeyReleased(FRKey k)
{
	if (k == FRKey::LEFT) {
		leftPressed = false;
	}
	if (k == FRKey::RIGHT) {
		rightPressed = false;
	}
}

const char* Engine::GetTitle()
{
	return "Arkanoid";
}

void showCursor(bool bShow) {
	ShowCursor(bShow);
}