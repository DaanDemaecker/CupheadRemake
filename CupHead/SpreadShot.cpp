#include "pch.h"
#include "SpreadShot.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

SpreadShot::SpreadShot(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, bool weak)
	:Bullet{ center, angle, damageMultiplier, 5 }, m_Weak{weak}
{
	if (weak)m_Speed = 800;
	else m_Speed = 1200;
	m_Damage = 1.24f;

	m_Range = 400.f;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 1, 4, 0);
}

SpreadShot::~SpreadShot()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void SpreadShot::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void SpreadShot::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);

		m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
		m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;

		CheckBoundaries();

		if (!level->IsInFrame(m_HitBox.center))
		{
			m_IsActivated = false;
		}
	}
}

void SpreadShot::Hit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::SpreadShot_Death);
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::SpreadShot_Death_Range);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
}

float SpreadShot::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void SpreadShot::CheckBoundaries()
{
	if (utils::GetDistance(m_HitBox.center, m_StartLocation) >= m_Range)
	{
		m_IsActivated = false;
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::SpreadShot_Death_Range);
	}
}