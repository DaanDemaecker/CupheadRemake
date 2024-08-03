#include "pch.h"
#include "ParticleSpriteSheet.h"
#include "utils.h"

ParticleSpriteSheet::ParticleSpriteSheet(Texture* texture, int nrCols, int nrRows, int nrFrames, const Point2f& pos, bool facingRight, float frameSpeed)
	:m_Pos{pos}, m_FacingRight{facingRight}, m_Angle{}, m_pTexture{texture}, 
	m_NrCols{nrCols}, m_NrRows{nrRows}, m_NrFrames{nrFrames}, m_FrameSpeed{frameSpeed}
{

}

ParticleSpriteSheet::ParticleSpriteSheet(Texture* texture, int nrCols, int nrRows, int nrFrames, const Point2f& pos, float angle, float frameSpeed)
	:m_Pos{ pos }, m_FacingRight{ true }, m_Angle{float(angle/M_PI*180)}, m_pTexture{ texture },
	m_NrCols{ nrCols }, m_NrRows{ nrRows }, m_NrFrames{ nrFrames }, m_FrameSpeed{ frameSpeed }
{

}

ParticleSpriteSheet::ParticleSpriteSheet(ParticleEntry* entry, const Point2f& pos, bool facingRight)
	:m_Pos{pos}, m_FacingRight{facingRight}, m_Angle{}, m_pTexture{entry->pSpriteSheet}, m_NrCols{entry->nrCols},
	m_NrRows{entry->nrRows}, m_NrFrames{entry->nrFrames}, m_FrameSpeed{entry->frameSpeed}
{
}

ParticleSpriteSheet::ParticleSpriteSheet(ParticleEntry* entry, const Point2f& pos, float angle)
	: m_Pos{ pos }, m_FacingRight{ true }, m_Angle{float(angle/M_PI*180)}, m_pTexture{entry->pSpriteSheet}, m_NrCols{entry->nrCols},
	m_NrRows{ entry->nrRows }, m_NrFrames{ entry->nrFrames }, m_FrameSpeed{ entry->frameSpeed }
{
}


ParticleSpriteSheet::~ParticleSpriteSheet()
{
	m_pTexture = nullptr;
}

void ParticleSpriteSheet::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_NrCols;
	srcRect.height = m_pTexture->GetHeight() / m_NrRows;
	srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
	srcRect.bottom = (m_CrFrame / m_NrCols + 1) * srcRect.height;

	glPushMatrix();
	glTranslatef( m_Pos.x, m_Pos.y, 0);
	glScalef(float(int(m_FacingRight) * 2 - 1), 1, 1);
	glRotatef(m_Angle, 0, 0, 1);
	glTranslatef(-m_pTexture->GetWidth() / (2*m_NrCols), -m_pTexture->GetHeight() / (2*m_NrRows), 0);
	m_pTexture->Draw(Point2f{}, srcRect);
	glPopMatrix();

	//utils::FillEllipse(m_Pos, 5, 5);
}

void ParticleSpriteSheet::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		NextFrame();
	}
}

bool ParticleSpriteSheet::GetFinishedCycle()
{
	return m_FinishedCycle;
}

void ParticleSpriteSheet::ResetCycle()
{
	m_FinishedCycle = false;
	m_CrFrame = 0;
}

void ParticleSpriteSheet::NextFrame()
{
	if (!m_FinishedCycle)m_CrFrame++;
	else m_CrFrame = m_NrFrames - 1;
	if (m_CrFrame >= m_NrFrames - 1)
	{
		m_FinishedCycle = true;
	}
}
