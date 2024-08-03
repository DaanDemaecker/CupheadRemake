#include "pch.h"
#include "BigChaliceBomb.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
extern ParticleSoundeffectManager* g_pSFXManager;

BigChaliceBomb::BigChaliceBomb(Texture* texturePtr, const Point2f& center, float damageMultiplier)
	:BigBullet(center, 0, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet(texturePtr, 6, 3, 16, 4, 1 / 24.f);

	m_Speed = Vector2f{ 1500.f, 0 };
}

BigChaliceBomb::~BigChaliceBomb()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigChaliceBomb::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, atan2f(m_Speed.y, m_Speed.x) / float(M_PI) * 180);
	}
}

void BigChaliceBomb::Update(float elapsedSec, const Level* level, const Rectf& player)
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
			m_HitBox.center.x += m_Speed.x * elapsedSec;
			m_HitBox.center.y += m_Speed.y * elapsedSec;

			m_Speed.y += m_Gravity * elapsedSec;

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
				m_HitBox.center.x += m_Speed.x  / 10.f * elapsedSec;
				m_HitBox.center.y += m_Speed.y  / 10.f * elapsedSec;

				m_Speed.y += m_Gravity / 10.f * elapsedSec;
			}
			else
			{
				m_HitBox.center.x += m_Speed.x / 4.f * elapsedSec;
				m_HitBox.center.y += m_Speed.y / 4.f * elapsedSec;

				m_Speed.y += m_Gravity / 4.f * elapsedSec;
			}
		}
	}
}

void BigChaliceBomb::CheckBoundaries()
{
	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		BulletHit();
	}
}

void BigChaliceBomb::BulletHit()
{
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::BigBullet_Hit);

	m_CanHit = false;
}


float BigChaliceBomb::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

bool BigChaliceBomb::Hit()
{
	if (m_CanHit)
	{
		BulletHit();
		return true;
	}
	return false;
}