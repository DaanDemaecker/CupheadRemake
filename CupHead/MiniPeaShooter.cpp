#include "pch.h"
#include "MiniPeaShooter.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

MiniPeaShooter::MiniPeaShooter(Texture* texturePtr, const Point2f& center, float damageMultiplier)
	:Bullet{ center, 0, damageMultiplier, 5 }
{
	m_Speed = 1600;
	m_Damage = 2;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 7, 0);
}

MiniPeaShooter::~MiniPeaShooter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void MiniPeaShooter::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);

		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		//utils::DrawEllipse(m_HitBox.center, m_HitBox.radius, m_HitBox.radius, 5);
	}
}

void MiniPeaShooter::Update(float elapsedSec, const Level* level)
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

void MiniPeaShooter::Hit()
{
	m_IsActivated = false;
	//g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::PeaShooter_Death);
	//g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
}

float MiniPeaShooter::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void MiniPeaShooter::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_Range, 2))
	{
		Hit();
	}
}