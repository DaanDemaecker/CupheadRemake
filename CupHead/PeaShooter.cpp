#include "pch.h"
#include "PeaShooter.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

PeaShooter::PeaShooter(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:Bullet{center, angle, damageMultiplier, 5}
{
	m_Speed = 1300;
	m_Damage = 4;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 2);
}

PeaShooter::~PeaShooter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PeaShooter::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void PeaShooter::Update(float elapsedSec, const Level* level)
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

void PeaShooter::Hit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::PeaShooter_Death);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
}

float PeaShooter::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void PeaShooter::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}