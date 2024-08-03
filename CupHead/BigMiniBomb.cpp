#include "pch.h"
#include "BigMiniBomb.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

BigMiniBomb::BigMiniBomb(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, std::weak_ptr<Point2f> target)
	:BigBullet{ center, angle, damageMultiplier, 5 }
{
	m_pTarget = target;
	m_Speed = 1000;
	m_Damage = 6.f;
	m_pTarget = target;
	m_pTexture = new BulletSpriteSheet(texturePtr, 5, 2, 10, 0);
}

BigMiniBomb::~BigMiniBomb()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigMiniBomb::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void BigMiniBomb::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);

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

bool BigMiniBomb::Hit()
{
	m_IsActivated = false;

	return true;
}

float BigMiniBomb::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void BigMiniBomb::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}