#include "pch.h"
#include "PlanePeaShooter.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

PlanePeaShooter::PlanePeaShooter(Texture* texturePtr, const Point2f& center, float damageMultiplier)
	:Bullet{ center, 0, damageMultiplier, 5 }
{
	m_Speed = 1300;
	m_Damage = 4;

	m_pTexture = new BulletSpriteSheet(texturePtr, 5, 1, 5, 1);
}

PlanePeaShooter::~PlanePeaShooter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PlanePeaShooter::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);

		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		//utils::DrawEllipse(m_HitBox.center, m_HitBox.radius, m_HitBox.radius, 5);
	}
}

void PlanePeaShooter::Update(float elapsedSec, const Level* level)
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

void PlanePeaShooter::Hit()
{
	m_IsActivated = false;
	//g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::PeaShooter_Death);
	//g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
}

float PlanePeaShooter::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void PlanePeaShooter::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}