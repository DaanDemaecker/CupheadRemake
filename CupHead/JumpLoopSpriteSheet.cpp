#include "pch.h"
#include "JumpLoopSpriteSheet.h"
#include "utils.h"

int JumpLoopSpriteSheet::m_CurrentFrame{};

bool JumpLoopSpriteSheet::m_IsInFirstLoop{false};
bool JumpLoopSpriteSheet::m_IsInSecondLoop{ false };

bool JumpLoopSpriteSheet::m_Looping1{ true };
bool JumpLoopSpriteSheet::m_Looping2{ true };

JumpLoopSpriteSheet::JumpLoopSpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, int firstLoopStart, int firstLoopStop, int secondLoopStart, int secondLoopStop, float frameSpeed)
	:SpriteSheet(textpath, nrCols, nrRows, nrFrames, frameSpeed),
	m_FirstLoopStart{ firstLoopStart }, m_FirstLoopStop{ firstLoopStop },
	m_SecondLoopStart{secondLoopStart}, m_SecondLoopStop{secondLoopStop}
{

}

void JumpLoopSpriteSheet::DrawBottom(const Point2f& pos, bool facingRight, int mode)
{
	Rectf destRect{};
	destRect.width = m_pTexture->GetWidth() / m_NrCols;
	destRect.height = (m_pTexture->GetHeight() / m_NrRows);
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


void JumpLoopSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

void JumpLoopSpriteSheet::NextFrame()
{
	m_CurrentFrame++;

	if (m_Looping1 && m_CrFrame > m_FirstLoopStop)m_CurrentFrame = m_FirstLoopStart;
	else if (m_Looping2 && m_CrFrame > m_SecondLoopStop)m_CurrentFrame = m_SecondLoopStart;
	else if (!m_Looping1 && !m_Looping2 && m_CurrentFrame >= m_NrFrames)
	{
		m_CurrentFrame = 0;
		m_Looping1 = true;
		m_Looping2 = true;
	}
}

void JumpLoopSpriteSheet::ResetCycle()
{
	m_CurrentFrame = 0;
	m_Looping1 = true;
	m_Looping2 = true;
	m_IsInFirstLoop = true;
	m_IsInSecondLoop = false;
}

void JumpLoopSpriteSheet::GoToSecondLoop()
{
	m_Looping1 = false;
	m_IsInSecondLoop = true;
	m_IsInFirstLoop = false;
	m_CurrentFrame = m_SecondLoopStart;
}

bool JumpLoopSpriteSheet::IsLooping1()
{
	return m_IsInFirstLoop;
}

bool JumpLoopSpriteSheet::IsLooping2()
{
	return m_IsInSecondLoop;
}