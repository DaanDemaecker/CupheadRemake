#include "pch.h"
#include "Lobber.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include "OneCycleSpriteSheet.h"

extern ParticleSoundeffectManager* g_pSFXManager;

Lobber::Lobber(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:Bullet{ center, angle, damageMultiplier, 15 }
{
	m_Speed = Vector2f{ cosf(angle)*800,sinf(angle)*800 + 300.f};
	m_Damage = 11.6f;

	m_pTexture = new BulletSpriteSheet(texturePtr, 6, 2, 11, 0);

	m_pBounce1 = new OneCycleSpriteSheet("./Resources/images/player/bullets/lobber/Lobber_Bounce1.png", 2, 1, 2);
	m_pBounce2 = new OneCycleSpriteSheet("./Resources/images/player/bullets/lobber/Lobber_Bounce2.png", 2, 1, 2);
}

Lobber::~Lobber()
{
	delete m_pTexture;
	m_pTexture = nullptr;
	delete m_pBounce1;
	m_pBounce1 = nullptr;
	delete m_pBounce2;
	m_pBounce2 = nullptr;
}

void Lobber::Draw() const
{
	if (m_IsActivated)
	{
		switch (m_State)
		{
		case Lobber::State::flying:
			m_pTexture->Draw(m_HitBox.center, 0);
			break;
		case Lobber::State::bounce1:
			m_pBounce1->DrawBottom(m_HitBox.center);
			break;
		case Lobber::State::bounce2:
			m_pBounce2->DrawBottom(m_HitBox.center);
			break;
		default:
			break;
		}

	}
}

void Lobber::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{
		switch (m_State)
		{
		case Lobber::State::flying:
			m_pTexture->Update(elapsedSec);
			m_Speed.y += m_Gravity * elapsedSec;

			if (m_Speed.y >= m_MaxSpeed)m_Speed.y = m_MaxSpeed;
			else if (m_Speed.y <= -m_MaxSpeed)m_Speed.y = -m_MaxSpeed;

			m_HitBox.center.x += m_Speed.x * elapsedSec;
			m_HitBox.center.y += m_Speed.y * elapsedSec;
			break;
		case Lobber::State::bounce1:
			m_pBounce1->Update(elapsedSec);
			m_HitBox.center.x += m_Speed.x * elapsedSec;
			if (m_pBounce1->GetFinishedCycle())
			{
				m_State = State::flying;
				m_Speed.y *= -1.f;
			}
			break;
		case Lobber::State::bounce2:
			m_pBounce2->Update(elapsedSec);
			if (m_pBounce2->GetFinishedCycle())
			{
				m_State = State::flying;
				m_Speed.y *= -1.f;
			}
			break;
		default:
			break;
		}

		CheckBoundaries(level);
	}
}

void Lobber::Hit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Lobber_Death);
}

float Lobber::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void Lobber::CheckBoundaries(const Level* level)
{
	if (level->IsOnGround(m_HitBox.center))
	{
		if (m_State == State::flying)
		{
			if (!m_pBounce1->GetFinishedCycle())m_State = State::bounce1;
			else if (m_Speed.y < 0 && !m_pBounce2->GetFinishedCycle())m_State = State::bounce2;
			else if (m_Speed.y < 0)
			{
				m_IsActivated = false;
				g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Lobber_Death);
			}
		}
	}

	if (pow(m_StartLocation.x - m_HitBox.center.x, 2) >= pow(m_WindowWidth*2, 2)
		|| pow(m_StartLocation.y - m_HitBox.center.y, 2) >= pow(m_WindowHeight, 2))
	{
		m_IsActivated = false;
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Lobber_Death);
	}

}