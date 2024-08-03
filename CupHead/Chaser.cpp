#include "pch.h"
#include "Chaser.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

Chaser::Chaser(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, std::weak_ptr<Point2f> target)
	:Bullet{ center, angle, damageMultiplier, 5 }
{
	m_pTrail = new SpriteSheet{ "./resources/Images/Player/Bullets/Chaser/Chaser_Trail.png", 6, 2, 12 };

	m_pTarget = target;
	m_Speed = 1000;
	m_Damage = 2.6f;
	m_pTarget = target;
	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 0);
}

Chaser::~Chaser()
{
	delete m_pTrail;
	m_pTrail = nullptr;
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Chaser::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
		m_pTrail->DrawCenter(Point2f{ m_HitBox.center.x - 30 * cosf(m_Angle), m_HitBox.center.y - 30 * sinf(m_Angle) });
	}
}

void Chaser::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);
		m_pTrail->Update(elapsedSec);

		if (!m_TargetExpired)m_TargetExpired = m_pTarget.expired();

		if (!m_TargetExpired)
		{
			utils::RotateObjectToTarget(m_HitBox.center, m_Angle, *m_pTarget.lock(), m_RotSpeed * elapsedSec);
		}

		m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
		m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;

		CheckBoundaries();

		if (!level->IsInFrame(m_HitBox.center))
		{
			m_IsActivated = false;
		}
	}
}

void Chaser::Hit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Chaser_Death);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chaser_Death);
}

float Chaser::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void Chaser::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}