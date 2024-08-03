#pragma once
#include "SpriteSheet.h"

class LoopSpriteSheet : public SpriteSheet
{
//Spritesheet class that loops the given frames instead of all the frames

public:
	LoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int loopStart, int loopStop ,float frameSpeed = 1 / 20.f);
	virtual void Update(float elapsedSec) override;

	void ResetCycle();
	void StopLoop();

	bool IsLooping();
private:

	int m_LoopStart;
	int m_LoopStop;

	bool m_Looping{ true };

	bool m_IsInLoop{ false };

	void NextFrame();

};

