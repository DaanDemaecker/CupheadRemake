#include "pch.h"
#include "SpecialAttackSpriteSheet.h"
#include "utils.h"
#include <iostream>

int SpecialAttackSpriteSheet::m_Angle = 0;

SpecialAttackSpriteSheet::SpecialAttackSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed)
{
	
}

void SpecialAttackSpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
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

void SpecialAttackSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void SpecialAttackSpriteSheet::CalculateSourceRect(Rectf& srsRect)
{
	srsRect.width = m_pTexture->GetWidth() / m_NrCols;
	srsRect.height = m_pTexture->GetHeight() / m_NrRows;

	srsRect.bottom = (int(m_Angle) + 1) * srsRect.height;

	srsRect.left = m_CrFrame * srsRect.width;
}

void SpecialAttackSpriteSheet::UpdateAngle(int angle)
{
	m_Angle = angle;
}

bool SpecialAttackSpriteSheet::GetFinishedCycle()
{
	return m_FinishedCycle;
}

void SpecialAttackSpriteSheet::ResetCycle()
{
	m_FinishedCycle = false;
	m_CrFrame = 0;
}

void SpecialAttackSpriteSheet::NextFrame()
{
	if (m_CrFrame < m_NrCols-1)m_CrFrame++;
	else m_LastFrameShown++;

	if (m_LastFrameShown >= 2)
	{
		m_FinishedCycle = true;
		m_LastFrameShown = 0;
	}
}

int SpecialAttackSpriteSheet::GetCrFrame()
{
	return m_CrFrame;
}