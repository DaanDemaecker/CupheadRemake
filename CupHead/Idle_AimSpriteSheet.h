#pragma once
#include "SpriteSheet.h"

class Idle_AimSpriteSheet final : public SpriteSheet
{
//Spritesheet used for idle and aim animation
//Seperate class was needed for the 5 different directions the player can face
//And a Static currentframe is used to make transition between states smoother

public:
	Idle_AimSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1 / 10.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0)override;
	virtual void Update(float elapsedSec);
	void UpdateAngle(int angle);
private:
	int m_Angle{};


	static bool m_OscillatingRight;
	static int m_CurrentFrame;

	void NextFrame();
	void CalculateSourceRect(Rectf& srsRect);

};

