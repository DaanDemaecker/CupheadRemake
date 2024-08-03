#include "pch.h"
#include "BigLobber.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
extern ParticleSoundeffectManager* g_pSFXManager;

BigLobber::BigLobber(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:BigBullet(center, angle, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 0, 1 / 15.f);

	m_Speed = Vector2f{ cosf(angle) * 750.f ,sinf(angle) * 750.f + 200 };
}

BigLobber::~BigLobber()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigLobber::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, 0); //m_Angle / float(M_PI) * 180);
	}
}

void BigLobber::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);

		m_TrailTimer += elapsedSec;
		if (m_TrailTimer >= m_TrailTime)
		{
			m_TrailTimer -= m_TrailTime;
			m_TrailTime = (rand() % 5 + 8) / 100.f;

			Point2f point{ m_HitBox.center.x + (rand() % 40 - 20), m_HitBox.center.y + (rand() % 40 - 20) };
			g_pSFXManager->CreateParticle(point, ParticleSoundeffectManager::ParticleType::Lobber_Special_Trail);
		}

		m_HitBox.center.x += m_Speed.x * elapsedSec;
		m_HitBox.center.y += m_Speed.y * elapsedSec;
		m_Speed.y += m_Gravity * elapsedSec;

		CheckBoundaries();

		if (!level->IsInFrame(m_HitBox.center))
		{
			m_IsActivated = false;
		}

	}
}

void BigLobber::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		BulletHit();
	}
}

void BigLobber::BulletHit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Lobber_Special_Death);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Explosion);
}

float BigLobber::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

Enums::BulletType BigLobber::GetBulletType() const
{
	return Enums::BulletType::Lobber;
}

bool BigLobber::Hit()
{
	BulletHit();
	return true;
}