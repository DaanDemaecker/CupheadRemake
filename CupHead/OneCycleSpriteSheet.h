#pragma once
#include "SpriteSheet.h"

class OneCycleSpriteSheet : public SpriteSheet
{
//Spritesheet class that stops after going trought the animation once

public:
	OneCycleSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1 / 20.f);

	virtual void Update(float elapsedSec);
	bool GetFinishedCycle();
	void ResetCycle();

	int GetFrame();
private:
	bool m_FinishedCycle{ false };

	void NextFrame();
};

