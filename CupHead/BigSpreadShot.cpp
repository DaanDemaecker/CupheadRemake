#include "pch.h"
#include "BigSpreadShot.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
extern ParticleSoundeffectManager* g_pSFXManager;

BigSpreadShot::BigSpreadShot(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:BigBullet(center, angle, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet{texturePtr, 5, 2, 9, 5, 1/24.f};

}

BigSpreadShot::~BigSpreadShot()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigSpreadShot::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void BigSpreadShot::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		m_TrailTimer += elapsedSec;
		if (m_TrailTimer >= m_TrailTime)
		{
			m_TrailTimer -= m_TrailTime;
			g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::SpreadShot_Trail, m_Angle);
		}


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

void BigSpreadShot::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		BulletHit();
	}
}

void BigSpreadShot::BulletHit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::SpreadShot_Special_Death);
}


float BigSpreadShot::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

Enums::BulletType BigSpreadShot::GetBulletType() const
{
	return Enums::BulletType::SpreadShot;
}

bool BigSpreadShot::Hit()
{

	BulletHit();
	return true;

}