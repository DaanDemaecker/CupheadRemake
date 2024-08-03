#pragma once
#include "SpriteSheet.h"

class JumpSpriteSheet final :  public SpriteSheet
{
//Spritesheet class used for parrying and jumping
//Seperate class needed for static variable to make transition between states smoother

public:
	JumpSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1 / 25.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0)override;
	virtual void Update(float elapsedSec) override;
	void ResetCycle();
private:


	static int m_CurrentFrame;
	void NextFrame();
};

