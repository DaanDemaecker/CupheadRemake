#include "pch.h"
#include "OneCycleSpriteSheet.h"


OneCycleSpriteSheet::OneCycleSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed)
{

}

void OneCycleSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

bool OneCycleSpriteSheet::GetFinishedCycle()
{
	return m_FinishedCycle;
}

void OneCycleSpriteSheet::ResetCycle()
{
	m_FinishedCycle = false;
	m_CrFrame = 0;
}

void OneCycleSpriteSheet::NextFrame()
{
	

	if (!m_FinishedCycle)m_CrFrame++;
	else m_CrFrame = m_NrFrames - 1;

	if (m_CrFrame >= m_NrFrames)
	{
		m_FinishedCycle = true;
	}
}

int OneCycleSpriteSheet::GetFrame()
{
	return m_CrFrame;
}