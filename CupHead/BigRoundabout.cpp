#include "pch.h"
#include "BigRoundabout.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

BigRoundabout::BigRoundabout(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:BigBullet(center, angle, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet(texturePtr, 5, 3, 14, 2, 1 / 15.f);

}

BigRoundabout::~BigRoundabout()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigRoundabout::Draw() const
{
	if (m_IsActivated)
	{
		DrawTrail();
		m_pTexture->Draw(m_HitBox.center, m_Angle / float(M_PI) * 180);
	}
}

void BigRoundabout::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		m_TrailTimer += elapsedSec;
		if (m_TrailTimer >= m_TrailTime)
		{
			m_TrailTimer -= m_TrailTime;
			m_Trail.push_back(m_HitBox.center);
			if (m_Trail.size() > m_TrailSize)
			{
				m_Trail2.push_back(m_Trail[0]);
				if (m_Trail2.size() > m_TrailSize)m_Trail2.pop_front();
				m_Trail.pop_front();
			}
		}


		m_pTexture->Update(elapsedSec);

		if (!m_CanHit)
		{
			m_HitTimer += elapsedSec;
			if (m_HitTimer >= m_HitDelay)
			{
				m_HitTimer = 0;
				m_CanHit = true;
			}
		}



		if (m_CanHit)
		{
			m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
			m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;
		}
		else
		{
			if (m_HitTimer <= m_HitDelay / 2)
			{
				m_HitBox.center.x += cosf(m_Angle) * m_Speed / 2.f * elapsedSec;
				m_HitBox.center.y += sinf(m_Angle) * m_Speed / 2.f * elapsedSec;
			}
			else
			{
				m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
				m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;
			}
		}

		if (utils::IsPointInRect(m_HitBox.center, player))
		{
			m_IsActivated = false;
			g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Roundabout_Special_Death, m_Angle);
		}

		m_Speed -= (int(m_Direction) * 2 - 1) * m_Acceleration * elapsedSec;

		CheckTurn(player);
		
	}
}

void BigRoundabout::BulletHit()
{
	m_Hits++;

	if (m_Hits != m_MaxHits)
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::BigBullet_Hit);
		m_CanHit = false;
	}
	else
	{
		m_IsActivated = false;
		m_Hits = 0;
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Roundabout_Special_Death);
	}
}

float BigRoundabout::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

Enums::BulletType BigRoundabout::GetBulletType() const
{
	return Enums::BulletType::Roundabout;
}

bool BigRoundabout::Hit()
{
	if (m_CanHit)
	{
		BulletHit();
		return true;
	}
	return false;
}

void BigRoundabout::CheckTurn(const Rectf& player)
{
	float marge{ 20 };

	if (m_Direction)
	{
		if (m_Speed <= -1 * m_MiddleSpeed)
		{
			m_Direction = false;
		}

		if (!m_NearZero && m_Speed < marge && m_Speed > -marge)
		{
			Point2f playerCenter{player.left + player.width / 2 ,  player.bottom};
			m_NearZero = true;
			m_Speed = 0;
			m_Angle = atan2f(m_HitBox.center.y - playerCenter.y, m_HitBox.center.x - playerCenter.x);
		}
		else if (m_NearZero && (m_Speed > marge || m_Speed < -marge))
		{
			m_NearZero = false;
		}
	}
	else
	{
		if (m_Speed >= 1 * m_MiddleSpeed)
		{
			m_Direction = true;
		}

		if (!m_NearZero && m_Speed < marge && m_Speed > -marge)
		{
			Point2f playerCenter{ player.left + player.width / 2, player.bottom };
			m_NearZero = true;
			m_Speed = 0;
			m_Angle = atan2f(m_HitBox.center.y - playerCenter.y, m_HitBox.center.x - playerCenter.x) + float(M_PI);
		}
		else if (m_NearZero && (m_Speed > marge || m_Speed < -marge))
		{
			m_NearZero = false;
		}
	}
}

void BigRoundabout::DrawTrail() const
{
	if (m_Trail2.size() >= m_TrailSize)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.4f });
		m_pTexture->DrawTransparent(m_Trail2[0], m_Angle);
	}

	if (m_Trail.size() >= m_TrailSize)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.8f });
		m_pTexture->DrawTransparent(m_Trail[0], m_Angle);
	}

}