#include "pch.h"
#include "Idle_AimSpriteSheet.h"
#include "utils.h"

int Idle_AimSpriteSheet::m_CurrentFrame{ 0 };

bool Idle_AimSpriteSheet::m_OscillatingRight{ true };

Idle_AimSpriteSheet::Idle_AimSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed)
{

}



void Idle_AimSpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
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

void Idle_AimSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void Idle_AimSpriteSheet::CalculateSourceRect(Rectf& srsRect)
{
	srsRect.width = m_pTexture->GetWidth() / m_NrCols;
	srsRect.height = m_pTexture->GetHeight() / m_NrRows;

	if (m_NrRows > 1)srsRect.bottom = (int(m_Angle) + 1) * srsRect.height;
	else srsRect.bottom = srsRect.height;

	srsRect.left = m_CurrentFrame * srsRect.width;

}

void Idle_AimSpriteSheet::UpdateAngle(int angle)
{
	m_Angle = angle;
}

void Idle_AimSpriteSheet::NextFrame()
{
	if (m_OscillatingRight && m_CurrentFrame == 4)m_OscillatingRight = false;
	else if (!m_OscillatingRight && m_CurrentFrame == 0)m_OscillatingRight = true;

	m_CurrentFrame += int(m_OscillatingRight) * 2 - 1;
}

