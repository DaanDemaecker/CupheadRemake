#pragma once
#include "SpriteSheet.h"

class ReturningLoopSpriteSheet : public SpriteSheet
{
	//Spritesheet class that loops the given frames instead of all the frames

public:
	ReturningLoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int loopStart, float frameSpeed = 1 / 20.f);
	virtual void Update(float elapsedSec) override;

	bool HasReturned();

	void ResetCycle();
	void ReturnCycle();
private:

	int m_LoopStart;

	bool m_Looping{ true };

	void NextFrame();

};

