#include "pch.h"
#include "ShootSpriteSheet.h"
#include "utils.h"

ShootSpriteSheet::ShootSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int loopStop, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed),m_LoopStop{loopStop}
{

}

void ShootSpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
{

	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = m_pTexture->GetHeight() / m_NrRows;
	destRect.bottom = 0;
	destRect.left = 0 - destRect.width / 2;

	Rectf srsRect{};
	CalculateSourceRect(srsRect);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(float(int(facingRight) * 2 - 1), 1, 1);
	m_pTexture->Draw(Rectf{ 0 - destRect.width / 2, 0, destRect.width, destRect.height }, srsRect, mode);
	glPopMatrix();

}

void ShootSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void ShootSpriteSheet::CalculateSourceRect(Rectf& srsRect)
{
	srsRect.width = m_pTexture->GetWidth() / m_NrCols;
	srsRect.height = m_pTexture->GetHeight() / m_NrRows;

	srsRect.bottom = (int(m_Angle) + 1) * srsRect.height;

	srsRect.left = m_CrFrame * srsRect.width;
}

void ShootSpriteSheet::UpdateAngle(int angle)
{
	m_Angle = angle;
}

void ShootSpriteSheet::NextFrame()
{
	m_CrFrame++;

	if (m_Looping)
	{
		if (m_CrFrame >= m_LoopStop)m_CrFrame = 0;
	}
	else
	{
		if (m_CrFrame >= m_NrCols)
		{
			m_Looping = true;
			m_CrFrame = 0;
		}
	}
}

void ShootSpriteSheet::StopLoop()
{
	m_Looping = false;
}

