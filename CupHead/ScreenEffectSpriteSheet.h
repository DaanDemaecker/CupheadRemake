#pragma once
#include "SpriteSheet.h"
class ScreenEffectSpriteSheet final : public SpriteSheet
{
//Spritesheet class used for frames that take up the whole window

public:
	ScreenEffectSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, bool loop, float frameSpeed = 1 / 20.f);

	void Draw(const Rectf& destrect);
	virtual void Update(float elapsedSec);
	bool GetFinishedCycle();
	void ResetCycle(bool goingRight = true);
private:
	bool m_FinishedCycle{ false };
	bool m_GoingRight{ true };

	bool m_Loop;

	void NextFrame();
};

