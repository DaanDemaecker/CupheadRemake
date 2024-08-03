#include "pch.h"
#include "BulletSpriteSheet.h"
#include "utils.h"
#include <iostream>

BulletSpriteSheet::BulletSpriteSheet(Texture* texturePtr, int nrCols, int nrRows, int nrFrames, int loopStart, float frameSpeed)
	:m_pTexture{texturePtr}, m_NrCols{nrCols},  m_NrRows{nrRows}, m_NrFrames{nrFrames}, m_LoopStart{loopStart}, m_FrameSpeed{frameSpeed}
{

}

void BulletSpriteSheet::Draw(const Point2f& pos, float angle) const
{

	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = m_pTexture->GetHeight() / m_NrRows;
	destRect.bottom = -destRect.height/2;
	destRect.left =  -3* destRect.width / 4;

	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_NrCols;
	srcRect.height = m_pTexture->GetHeight() / m_NrRows;
	srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
	srcRect.bottom = (m_CrFrame / m_NrCols + 1) * srcRect.height;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(angle, 0, 0, 1);
	m_pTexture->Draw(destRect, srcRect);
	glPopMatrix();
}

void BulletSpriteSheet::DrawTransparent(const Point2f& pos, float angle) const
{

	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = m_pTexture->GetHeight() / m_NrRows;
	destRect.bottom = -destRect.height / 2;
	destRect.left = -3 * destRect.width / 4;

	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_NrCols;
	srcRect.height = m_pTexture->GetHeight() / m_NrRows;
	srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
	srcRect.bottom = (m_CrFrame / m_NrCols + 1) * srcRect.height;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(angle, 0, 0, 1);
	m_pTexture->Draw(destRect, srcRect, 2);
	glPopMatrix();
}

void BulletSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

int BulletSpriteSheet::GetCrFrame()
{
	return m_CrFrame;
}

void BulletSpriteSheet::NextFrame()
{
	if ( ++m_CrFrame >= m_NrFrames)
	{
		m_CrFrame = m_LoopStart;
	}
}