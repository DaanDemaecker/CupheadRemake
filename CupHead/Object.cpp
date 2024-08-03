#include "pch.h"
#include "Object.h"
#include "Level.h"
#include "utils.h"
#include "LoopSpriteSheet.h"
#include <cmath>

extern ParticleSoundeffectManager* g_pSFXManager;

Object::Object(std::string textpath, ParticleSoundeffectManager::ParticleType  deathParticle, ParticleSoundeffectManager::SFXType deathSFX, int nrFrames, int loopStart, bool damaging, bool parryable, bool parryDestroys)
	:m_Damaging{damaging}, m_Parryable{parryable}, m_ParryDestroys{parryDestroys},
	m_DeathParticle{deathParticle}, m_DeathSFX{deathSFX}
{
	m_pSpriteSheet = new LoopSpriteSheet{ textpath, nrFrames, 1 , nrFrames, loopStart, nrFrames-1 , 1/10.f};
	m_Radius = m_pSpriteSheet->GetFrameWidth() / 2;
}

Object::~Object()
{
	m_Center = Point2f{0.f, 0.f};
	delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;
}

void Object::Draw()
{
	if (m_Activated)
	{	
		m_pSpriteSheet->DrawCenter(Point2f{ m_Center });
	}
}

void Object::Update(float elapsedSec, const Level* level)
{
	if (m_Activated)
	{
		m_Center.x += cosf(m_Angle) * m_Speed * elapsedSec;
		m_Center.y += sinf(m_Angle) * m_Speed * elapsedSec;
		m_pSpriteSheet->Update(elapsedSec);

		if (!level->IsInFrame(m_Center) && !level->IsInLevel(m_Center))
		{
			m_Activated = false;
		}
	}

}

void Object::SetObject(const Point2f& center, float angle, float speed)
{
	m_Activated = true;
	m_Center = center;
	m_Angle = angle;
	m_Speed = speed;
	m_pSpriteSheet->ResetCycle();
}

bool Object::CheckParry(const Rectf& shape)
{
	if (m_Activated && m_Parryable && utils::IsOverlapping(shape, Circlef{ m_Center, m_Radius }))
	{
		if (m_ParryDestroys)
		{
			m_Activated = false;
			g_pSFXManager->CreateParticle(m_Center, m_DeathParticle);
			g_pSFXManager->PlaySFX(m_DeathSFX);
		}
		return true;
	}
	else return false;
}

bool Object::CeckHit(const Rectf& shape, bool parry)
{
	if (m_Activated && m_Damaging && !parry && utils::IsOverlapping(shape, Circlef{ m_Center, m_Radius }))
	{
		m_Activated = false;
		g_pSFXManager->CreateParticle(m_Center, m_DeathParticle);
		g_pSFXManager->PlaySFX(m_DeathSFX);
		return true;
	}
	else return false;
}

Point2f Object::GetCenter()
{
	return m_Center;
}

bool Object::GetActivated()
{
	return m_Activated;
}

