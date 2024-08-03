#include "pch.h"
#include "LoopSpriteSheet.h"
#include "utils.h"

LoopSpriteSheet::LoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames,int loopStart, int loopStop, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed),
	m_LoopStart{loopStart}, m_LoopStop{loopStop}
{

}


void LoopSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}

	if (m_CrFrame >= m_LoopStart && m_CrFrame <= m_LoopStop)
	{
		m_IsInLoop = true;
	}
	else m_IsInLoop = false;

}

void LoopSpriteSheet::NextFrame()
{
	m_CrFrame++;

	if (m_Looping && m_CrFrame == m_LoopStop+1)m_CrFrame = m_LoopStart;
	else if (!m_Looping && m_CrFrame >= m_NrFrames)
	{
		m_CrFrame = 0;
		m_Looping = true;
	}
}

void LoopSpriteSheet::ResetCycle()
{
	m_CrFrame = 0;
	m_Looping = true;
}

void LoopSpriteSheet::StopLoop()
{
	m_Looping = false;
}

bool LoopSpriteSheet::IsLooping()
{
	return m_IsInLoop;
}