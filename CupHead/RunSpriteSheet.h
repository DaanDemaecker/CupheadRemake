#pragma once
#include "SpriteSheet.h"

class RunSpriteSheet final : public SpriteSheet
{
//Spritesheet class used for 3 types of running
//Seperate class needed for static member that makes transitions smoother

public:
	RunSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1 / 20.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0)override;
	virtual void Update(float elapsedSec) override;

	void ResetCycle();

	virtual int GetCrFrame() override;
private:


	static int m_CurrentFrame;
	void NextFrame();
};

