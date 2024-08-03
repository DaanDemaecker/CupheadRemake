#pragma once
#include "SpriteSheet.h"

class ShootSpriteSheet final : public SpriteSheet
{
//Spritesheet class used for shooting animations
//Seperate class was needed for the 5 different directions the player can face

public:
	ShootSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int loopStop, float frameSpeed = 1 / 20.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0)override;
	virtual void Update(float elapsedSec);
	void UpdateAngle(int angle);

	void StopLoop();
private:
	int m_Angle{};

	bool m_Looping{};
	int m_LoopStop{};

	void NextFrame();
	void CalculateSourceRect(Rectf& srsRect);

};

