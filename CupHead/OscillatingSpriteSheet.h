#pragma once
#include "SpriteSheet.h"

class OscillatingSpriteSheet final : public SpriteSheet
{
//Spritesheet class that oscillates the animation instead of looping

public:
	OscillatingSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1/20.f);

	void Update(float elapsedSec);
private:
	bool m_OscillatingRight{ false };

	void NextFrame();
};

