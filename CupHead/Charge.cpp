#include "pch.h"
#include "Charge.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

Charge::Charge(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, bool big)
	:Bullet{ center, angle, damageMultiplier, 5.f + int(big)*35.f}, m_Big{big}
{
	m_Speed = 1300;

	if (big)m_Damage = 52;
	else m_Damage = 4;

	if (m_Big) m_pTexture = new BulletSpriteSheet(texturePtr, 8, 2, 15, 3);
	else m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 0);
}

Charge::~Charge()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Charge::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void Charge::Update(float elapsedSec, const Level* level)
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

void Charge::Hit()
{
	m_IsActivated = false;

	if (m_Big)
	{
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Charge_Big_Death);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Charge_Big_Death);
	}
	else
	{
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Charge_Small_Death);
		//g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
	}

}

float Charge::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void Charge::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}