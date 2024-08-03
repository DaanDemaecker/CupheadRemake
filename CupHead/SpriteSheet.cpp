#include "pch.h"
#include "SpriteSheet.h"
#include "utils.h"
#include <iostream>

SpriteSheet::SpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed)
	:m_NrCols{ nrCols }, m_NrRows{ nrRows }, m_NrFrames{ nrFrames }, m_FrameSpeed{ frameSpeed }
{
	m_pTexture = new Texture(textpath);
}

SpriteSheet::~SpriteSheet()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void SpriteSheet::DrawPercentage(const Point2f& pos, float percentage)
{


	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = (m_pTexture->GetHeight() / m_NrRows)*percentage;
	destRect.bottom = 0;
	destRect.left = 0 - destRect.width / 2;

	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_NrCols;
	srcRect.height = m_pTexture->GetHeight() / m_NrRows;
	srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
	srcRect.bottom = (m_CrFrame / m_NrCols ) * srcRect.height + srcRect.height*percentage;
	srcRect.height *= percentage;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	m_pTexture->Draw(destRect, srcRect);
	//utils::DrawRect(destRect);
	glPopMatrix();

	//utils::FillEllipse(pos, 5, 5);


}

void SpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
{
	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = (m_pTexture->GetHeight() / m_NrRows);
	destRect.bottom = 0;
	destRect.left = 0 - destRect.width / 2;

	Rectf srcRect{};
	utils::CalculateSrcRect(m_pTexture, srcRect, m_NrCols, m_NrRows, m_CrFrame);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(float(int(facingRight) * 2 - 1), 1, 1);

	m_pTexture->Draw(destRect, srcRect, mode);
	glPopMatrix();
}

void SpriteSheet::DrawCenter(const Point2f& pos, bool facingRight, int mode)
{
	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = (m_pTexture->GetHeight() / m_NrRows);
	destRect.bottom = 0 - destRect.height/2;
	destRect.left = 0 - destRect.width / 2;

	Rectf srcRect{};
	utils::CalculateSrcRect(m_pTexture, srcRect, m_NrCols, m_NrRows, m_CrFrame);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(float(int(facingRight) * 2 - 1), 1, 1);

	m_pTexture->Draw(destRect, srcRect, mode);
	glPopMatrix();
}

void SpriteSheet::DrawCenterRotated(const Point2f& pos, float angle, int mode)
{
	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = (m_pTexture->GetHeight() / m_NrRows);
	destRect.bottom = 0 - destRect.height / 2;
	destRect.left = 0 - destRect.width / 2;

	Rectf srcRect{};
	utils::CalculateSrcRect(m_pTexture, srcRect, m_NrCols, m_NrRows, m_CrFrame);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(angle, 0, 0, 1);
	m_pTexture->Draw(destRect, srcRect, mode);
	glPopMatrix();
}

void SpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void SpriteSheet::NextFrame()
{
		m_CrFrame++;
		m_CrFrame %= m_NrFrames;
}

float SpriteSheet::GetFrameWidth() const
{
	return m_pTexture->GetWidth() / m_NrCols;
}

float SpriteSheet::GetFrameHeight() const
{
	return m_pTexture->GetHeight() / m_NrRows;
}

int SpriteSheet::GetCrFrame()
{
	return m_CrFrame;
}

void SpriteSheet::SetFrame(int frame)
{
	m_CrFrame = frame;
}
