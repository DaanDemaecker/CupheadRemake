#include "pch.h"
#include "ReturningLoopSpriteSheet.h"
#include "utils.h"

ReturningLoopSpriteSheet::ReturningLoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int loopStart, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed),
	m_LoopStart{ loopStart }
{

}


void ReturningLoopSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

bool ReturningLoopSpriteSheet::HasReturned()
{
	return (m_CrFrame == 0 && !m_Looping);
}

void ReturningLoopSpriteSheet::NextFrame()
{
	if (m_Looping)
	{
		m_CrFrame++;

		if (m_CrFrame == m_NrFrames)m_CrFrame = m_LoopStart;
	}
	else m_CrFrame--;
}

void ReturningLoopSpriteSheet::ResetCycle()
{
	m_CrFrame = 0;
	m_Looping = true;
}

void ReturningLoopSpriteSheet::ReturnCycle()
{
	m_Looping = false;
}