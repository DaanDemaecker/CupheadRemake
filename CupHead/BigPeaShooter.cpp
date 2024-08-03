#include "pch.h"
#include "BigPeaShooter.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
extern ParticleSoundeffectManager* g_pSFXManager;

BigPeaShooter::BigPeaShooter(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:BigBullet(center, angle, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 1, 1 / 15.f);

}

BigPeaShooter::~BigPeaShooter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigPeaShooter::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void BigPeaShooter::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		if (!m_CanHit)
		{
			m_HitTimer += elapsedSec;
			if (m_HitTimer >= m_HitDelay)
			{
				m_HitTimer = 0;
				m_CanHit = true;
			}
		}

		m_pTexture->Update(elapsedSec);

		if (m_CanHit)
		{
			m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
			m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;

			CheckBoundaries();

			if (!level->IsInFrame(m_HitBox.center))
			{
				m_IsActivated = false;
			}
		}
		else
		{
			if (m_HitTimer <= m_HitDelay / 2)
			{
				m_HitBox.center.x += cosf(m_Angle) * m_Speed / 10.f * elapsedSec;
				m_HitBox.center.y += sinf(m_Angle) * m_Speed / 10.f * elapsedSec;
			}
			else
			{
				m_HitBox.center.x += cosf(m_Angle) * m_Speed/4.f * elapsedSec;
				m_HitBox.center.y += sinf(m_Angle) * m_Speed/4.f * elapsedSec;
			}
		}
	}
}

void BigPeaShooter::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		BulletHit();
	}
}

void BigPeaShooter::BulletHit()
{
	m_Hits++;
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::BigBullet_Hit);

	if (m_Hits != m_MaxHits)
	{
		m_CanHit = false;
	}
	else
	{
		m_IsActivated = false;
		m_Hits = 0;
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::PeaShooter_Special_Death);
	}
}


float BigPeaShooter::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

Enums::BulletType BigPeaShooter::GetBulletType() const
{
	return Enums::BulletType::PeaShooter;
}

bool BigPeaShooter::Hit()
{
	if (m_CanHit)
	{
		BulletHit();
		return true;
	}
	return false;
}