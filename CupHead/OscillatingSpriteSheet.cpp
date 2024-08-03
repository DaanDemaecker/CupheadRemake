#include "pch.h"
#include "OscillatingSpriteSheet.h"
#include <iostream>

OscillatingSpriteSheet::OscillatingSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed)
{

}


void OscillatingSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void OscillatingSpriteSheet::NextFrame()
{
	if (m_OscillatingRight && m_CrFrame == m_NrFrames-1)m_OscillatingRight = false;
	else if (!m_OscillatingRight && m_CrFrame == 0)m_OscillatingRight = true;

	m_CrFrame += int(m_OscillatingRight) * 2 - 1;
}
