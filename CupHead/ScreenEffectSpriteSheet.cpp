#include "pch.h"
#include "ScreenEffectSpriteSheet.h"

ScreenEffectSpriteSheet::ScreenEffectSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, bool loop, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed), m_Loop{loop}
{

}

void ScreenEffectSpriteSheet::Draw(const Rectf& destRect)
{
	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_NrCols;
	srcRect.height = m_pTexture->GetHeight() / m_NrRows;
	srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
	srcRect.bottom = (m_CrFrame / m_NrCols + 1) * srcRect.height;

	m_pTexture->Draw(destRect, srcRect);

}

void ScreenEffectSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

bool ScreenEffectSpriteSheet::GetFinishedCycle()
{
	return m_FinishedCycle;
}

void ScreenEffectSpriteSheet::ResetCycle(bool goingRight)
{
	m_GoingRight = goingRight;
	m_FinishedCycle = false;
	if (goingRight)m_CrFrame = 0;
	else m_CrFrame = m_NrFrames - 1;
}

void ScreenEffectSpriteSheet::NextFrame()
{
	if (!m_Loop)
	{
		if (!m_FinishedCycle)m_CrFrame += int(m_GoingRight) * 2 - 1;
		else m_CrFrame = m_NrFrames - 1;

		if (m_GoingRight)
		{
			if (m_CrFrame >= m_NrFrames - 1)
			{
				m_FinishedCycle = true;
			}
		}
		else
		{
			if (m_CrFrame <= 0)
			{
				m_FinishedCycle = true;
			}
		}
	}
	else
	{
		++m_CrFrame %= m_NrFrames;
	}
}