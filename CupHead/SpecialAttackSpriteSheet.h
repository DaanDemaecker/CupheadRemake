#pragma once
#include "SpriteSheet.h"

class SpecialAttackSpriteSheet final : public SpriteSheet
{
//spritesheet class used for special attacks
//mix between one cycle spritesheets and shootspritesheets

public:
	explicit SpecialAttackSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1 / 20.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0) override;
	virtual void Update(float elapsedSec);
	void UpdateAngle(int angle);
	bool GetFinishedCycle();
	void ResetCycle();
	int GetCrFrame();
private:

	bool m_FinishedCycle{false};

	static int m_Angle;
	
	int m_LastFrameShown{};


	void NextFrame();
	void CalculateSourceRect(Rectf& srsRect);

};

