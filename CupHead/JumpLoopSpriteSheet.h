#pragma once
#include "SpriteSheet.h"

class JumpLoopSpriteSheet : public SpriteSheet
{
	//Spritesheet class that loops the given frames instead of all the frames

public:
	JumpLoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int firstLoopStart, int firstLoopStop, int secondLoopStart, int secondLoopStop, float frameSpeed = 1 / 20.f);
	virtual void Update(float elapsedSec) override;

	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0) override;

	void ResetCycle();

	void GoToSecondLoop();

	bool IsLooping1();
	bool IsLooping2();
private:

	int m_FirstLoopStart;
	int m_FirstLoopStop;

	int m_SecondLoopStart;
	int m_SecondLoopStop;

	void NextFrame();

	static int m_CurrentFrame;

	static bool m_Looping1;
	static bool m_Looping2;

	static bool m_IsInFirstLoop;
	static bool m_IsInSecondLoop;

};

