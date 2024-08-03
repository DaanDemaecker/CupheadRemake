#include "pch.h"
#include "BotanicPanic.h"
#include "SVGParser.h"
#include "Camera.h"

BotanicPanic::BotanicPanic(const Rectf& window)
	:Level{window,"./resources/Images/Levels/BotanicPanic/Level/BotanicPanic.svg" }
{
	m_pBackgroundTexture = new Texture("./resources/Images/Levels/BotanicPanic/Level/Background.png");
	m_pForegroundTexture = new Texture("./resources/Images/Levels/BotanicPanic/Level/Foreground.png");
	m_pOverlayTexture = new Texture("./resources/Images/Levels/BotanicPanic/Level/Overlay.png");

	m_pCamera = new Camera(window);

	m_pSkyeTexture = new Texture("./resources/images/Levels/BotanicPanic/Level/Sky.png");
	m_pCloud1Texture = new Texture("./resources/images/Levels/BotanicPanic/Level/Clouds1.png");
	m_pCloud2Texture = new Texture("./resources/images/Levels/BotanicPanic/Level/Clouds2.png");
	m_pMiddlegroundTexture = new Texture("./resources/Images/Levels/BotanicPanic/Level/Middleground.png");

	m_pTireSwingTexture = new OscillatingSpriteSheet("./resources/Images/Levels/BotanicPanic/Level/Tire_Swing.png", 6, 1, 6, 1/5.f);

	m_LevelBoundaries = Rectf{ 0, 0, m_pForegroundTexture->GetWidth(), m_pForegroundTexture->GetHeight() };
	m_pCamera->SetLevelBoundaries(m_LevelBoundaries);

	m_MiddleGroundFactor = (m_pForegroundTexture->GetWidth() - m_pBackgroundTexture->GetWidth()) /
		(m_pMiddlegroundTexture->GetWidth() - m_pBackgroundTexture->GetWidth());

	m_Cloud1Pos = 0;
	m_Cloud2Pos = m_Window.width;
}

BotanicPanic::~BotanicPanic()
{
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;
	delete m_pForegroundTexture;
	m_pForegroundTexture = nullptr;
	delete m_pOverlayTexture;
	m_pOverlayTexture = nullptr;

	delete m_pSkyeTexture;
	m_pSkyeTexture = nullptr;
	delete m_pCloud1Texture;
	m_pCloud1Texture = nullptr;
	delete m_pCloud2Texture;
	m_pCloud2Texture = nullptr;
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;
	delete m_pMiddlegroundTexture;
	m_pMiddlegroundTexture = nullptr;
	delete m_pForegroundTexture;
	m_pForegroundTexture = nullptr;
	delete m_pTireSwingTexture;
	m_pTireSwingTexture = nullptr;
}

void BotanicPanic::DrawLevel1() const
{
	m_pSkyeTexture->Draw(m_Window);

	glPushMatrix();
	glTranslatef(m_Cloud1Pos, 0, 0);
	m_pCloud1Texture->Draw(m_Window);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(m_Cloud2Pos, 0, 0);
	m_pCloud2Texture->Draw(m_Window);
	glPopMatrix();
}

void BotanicPanic::DrawLevel2() const
{
	m_pTireSwingTexture->DrawBottom(m_SwingPos);
	m_pBackgroundTexture->Draw(m_Window);
}

void BotanicPanic::DrawLevel3() const
{
	m_pMiddlegroundTexture->Draw();
}

void BotanicPanic::DrawLevel4() const
{
	m_pForegroundTexture->Draw();
}

void BotanicPanic::DrawLevel5() const
{
	m_pOverlayTexture->Draw(m_Window);
}

void BotanicPanic::UpdateLevel(float elapsedSec, const Rectf& player)
{
	m_Cloud1Pos -= m_CloudSpeed * elapsedSec;
	m_Cloud2Pos -= m_CloudSpeed * elapsedSec;

	if (m_Cloud1Pos <= -m_Window.width)m_Cloud1Pos += 2 * m_Window.width;
	if (m_Cloud2Pos <= -m_Window.width)m_Cloud2Pos += 2 * m_Window.width;

	m_pTireSwingTexture->Update(elapsedSec);
}

void BotanicPanic::UpdateCamera(float elapsedSec, const Rectf& target, float velocityX)
{
	m_pCamera->Update(elapsedSec, target, velocityX);
}

void BotanicPanic::TranslateCameraForeground() const
{
	m_pCamera->TranslateForeground();
}

void BotanicPanic::TranslateCameraMiddleground() const
{
	m_pCamera->TranslateMiddleground(m_MiddleGroundFactor);
}

Point2f BotanicPanic::GetCameraDifference() const
{
	return Point2f{ m_pCamera->GetCameraPos().x - m_pCamera->GetCameraPos().x / m_MiddleGroundFactor, 0 };
}

bool BotanicPanic::IsInFrame(const Point2f& pos) const
{
	const float border{ 50 };

	const Point2f cameraPos{ m_pCamera->GetCameraPos() };

	const Rectf frame{ Rectf{cameraPos.x - border, cameraPos.y - border, m_Window.width + 2 * border, m_Window.height + 2 * border} };

	return utils::IsPointInRect(pos, frame);
}