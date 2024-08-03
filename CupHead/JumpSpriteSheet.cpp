#include "pch.h"
#include "JumpSpriteSheet.h"
#include "utils.h"

int JumpSpriteSheet::m_CurrentFrame{};

JumpSpriteSheet::JumpSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed)
{

}

void JumpSpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
{
	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = m_pTexture->GetHeight() / m_NrRows;
	destRect.bottom = 0;
	destRect.left = 0 - destRect.width / 2;

	Rectf srcRect{};
	utils::CalculateSrcRect(m_pTexture, srcRect, m_NrCols, m_NrRows, m_CurrentFrame);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(float(int(facingRight) * 2 - 1), 1, 1);
	m_pTexture->Draw(destRect, srcRect, mode);
	glPopMatrix();
}

void JumpSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void JumpSpriteSheet::NextFrame()
{
	m_CurrentFrame++;
	m_CurrentFrame %= m_NrFrames;
}

void JumpSpriteSheet::ResetCycle()
{
	m_CurrentFrame = 0;
}