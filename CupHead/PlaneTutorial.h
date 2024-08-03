#pragma once
#include "Level.h"
class SpriteSheet;

class PlaneTutorial final : public Level
{
public:
	explicit PlaneTutorial(const Rectf& window);
	virtual ~PlaneTutorial() override;

	virtual void DrawLevel1() const override;
	virtual void DrawLevel2() const override;

	virtual void UpdateLevel(float elapsedSec, const Rectf& player) override;

	bool ShouldExit();

	virtual bool IsInFrame(const Point2f& pos) const;

private:
	Texture* m_pBackgroundTexture;
	SpriteSheet* m_pBackgroundAnimation;

	Texture* m_pExitSign;
	Rectf m_ExitBox{1038, 50, 113, 40};
	Point2f m_ExitSignPos{ 1070, 95 };
	bool m_PlayerInBox{ false };


};
