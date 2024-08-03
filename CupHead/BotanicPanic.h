#pragma once
#include "Level.h"
#include "Texture.h"
#include "OscillatingSpriteSheet.h"

class BotanicPanic final : public Level
{
public:
	explicit BotanicPanic(const Rectf& window);
	virtual ~BotanicPanic() override;

	virtual void UpdateLevel(float elapsedSec, const Rectf& player) override;
	virtual void UpdateCamera(float elapsedSec, const Rectf& target, float velocityX) override;

	virtual void DrawLevel1() const override;
	virtual void DrawLevel2() const override;
	virtual void DrawLevel3() const override;
	virtual void DrawLevel4() const override;
	virtual void DrawLevel5() const override;

	virtual void TranslateCameraMiddleground() const override;

	virtual void TranslateCameraForeground() const override;

	virtual Point2f GetCameraDifference() const override;

	virtual bool IsInFrame(const Point2f& pos) const override;

private:
	Texture* m_pSkyeTexture;
	Texture* m_pCloud1Texture;
	Texture* m_pCloud2Texture;
	Texture* m_pMiddlegroundTexture;

	Camera* m_pCamera;

	Texture* m_pBackgroundTexture;
	Texture* m_pForegroundTexture;
	Texture* m_pOverlayTexture;

	float m_MiddleGroundFactor;

	float m_Cloud1Pos;
	float m_Cloud2Pos;
	const float m_CloudSpeed{ 20.f };

	OscillatingSpriteSheet* m_pTireSwingTexture;
	const Point2f m_SwingPos{ 335, 435 };
};

