#include "pch.h"
#include "Tutorial.h"
#include "SVGParser.h"
#include "Camera.h"
#include "utils.h"

Tutorial::Tutorial(const Rectf& window)
	:Level(window, "./resources/Images/Levels/Tutorial/Tutorial.svg")
{
	m_pBackgroundTexture = new Texture("./resources/Images/Levels/Tutorial/Background.png");
	m_pForegroundTexture = new Texture("./resources/Images/Levels/Tutorial/Tutorial.png");
	m_pOverlayTexture = new Texture("./resources/Images/Levels/Tutorial/Foreground.png");

	m_pCamera = new Camera(window);

	m_pExitSign = new Texture{ "./resources/images/Levels/tutorial/Exit_Sign.png" };

	m_LevelBoundaries = Rectf{ -75, 0, m_pForegroundTexture->GetWidth() + 150, m_pForegroundTexture->GetHeight() };
	m_pCamera->SetLevelBoundaries(m_LevelBoundaries);
}

Tutorial::~Tutorial()
{
	delete m_pExitSign;
	m_pExitSign = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;
	delete m_pForegroundTexture;
	m_pForegroundTexture = nullptr;
	delete m_pOverlayTexture;
	m_pOverlayTexture = nullptr;
}

void Tutorial::DrawLevel2() const
{
	m_pBackgroundTexture->Draw(m_Window);
}

void Tutorial::DrawLevel4() const
{
	m_pForegroundTexture->Draw();

	if(m_PlayerPastLine)m_pExitSign->Draw(m_ExitSignPos);
}

void Tutorial::DrawLevel5() const
{
	m_pOverlayTexture->Draw(m_Window);
}

void Tutorial::TranslateCameraForeground() const
{
	m_pCamera->TranslateForeground(75.f);
}

void Tutorial::UpdateLevel(float elapsedSec, const Rectf& player)
{
	if (player.left >= m_ExitLine)m_PlayerPastLine = true;
	else m_PlayerPastLine = false;
}

void Tutorial::UpdateCamera(float elapsedSec, const Rectf& target, float velocityX)
{
	m_pCamera->Update(elapsedSec, target, velocityX);
}

bool Tutorial::ShouldExit()
{
	return m_PlayerPastLine;
}

bool Tutorial::IsInFrame(const Point2f& pos) const
{
	const float border{ 50 };

	const Point2f cameraPos{ m_pCamera->GetCameraPos() };

	const Rectf frame{ Rectf{cameraPos.x - border, cameraPos.y - border, m_Window.width + 2 * border, m_Window.height + 2 * border} };

	return utils::IsPointInRect(pos, frame);
}
