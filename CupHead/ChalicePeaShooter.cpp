#include "pch.h"
#include "ChalicePeaShooter.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

ChalicePeaShooter::ChalicePeaShooter(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:Bullet{ center, angle, damageMultiplier, 5 }
{
	m_Speed = 1300;
	m_Damage = 3.65f;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 1, 4, 0);
}

ChalicePeaShooter::~ChalicePeaShooter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void ChalicePeaShooter::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);

		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		//utils::DrawEllipse(m_HitBox.center, m_HitBox.radius, m_HitBox.radius, 5);
	}
}

void ChalicePeaShooter::Update(float elapsedSec, const Level* level)
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

void ChalicePeaShooter::Hit()
{
	m_IsActivated = false;
	//g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::PeaShooter_Death);
	//g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Death);
}

float ChalicePeaShooter::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void ChalicePeaShooter::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		Hit();
	}
}