#pragma once
#include "Level.h"
#include "Texture.h"

class Tutorial final: public Level
{
public:
	explicit Tutorial(const Rectf& window);
	virtual ~Tutorial() override;

	virtual void DrawLevel2() const override;
	virtual void DrawLevel4() const override;
	virtual void DrawLevel5() const override;

	virtual void TranslateCameraForeground() const override;

	virtual void UpdateLevel(float elapsedSec, const Rectf& player) override;
	virtual void UpdateCamera(float elapsedSec, const Rectf& target, float velocityX) override;
	bool ShouldExit();

	virtual bool IsInFrame(const Point2f& pos) const override;

private:
	float m_ExitLine{6080};
	Point2f m_ExitSignPos{6200, 350};

	Texture* m_pExitSign;
	bool m_PlayerPastLine{false};

	Camera* m_pCamera;

	Texture* m_pBackgroundTexture;
	Texture* m_pForegroundTexture;
	Texture* m_pOverlayTexture;
};

