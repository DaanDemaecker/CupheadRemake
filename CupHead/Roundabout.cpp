#include "pch.h"
#include "Roundabout.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"

extern ParticleSoundeffectManager* g_pSFXManager;

Roundabout::Roundabout(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:Bullet{ center, angle, damageMultiplier, 5 }, m_StartSpeed{1300}
{
	m_Speed = 1300;
	m_Damage = 8.5;

	m_Acceleration = 2500;

	m_pTexture = new BulletSpriteSheet(texturePtr, 5, 2, 10, 0);
}

Roundabout::~Roundabout()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Roundabout::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void Roundabout::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);

		m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
		m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;

		m_Speed -= m_Acceleration * elapsedSec;

		CheckBoundaries();

		if (!level->IsInFrame(m_HitBox.center) && m_Speed < -m_StartSpeed)
		{
			m_IsActivated = false;
		}
	}
}

void Roundabout::Hit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Roundabout_Death);
}

float Roundabout::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void Roundabout::CheckBoundaries()
{
	if (utils::GetDistance(m_StartLocation, m_HitBox.center) >= m_WindowWidth)
	{
		m_IsActivated = false;
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Roundabout_Death);
	}
}