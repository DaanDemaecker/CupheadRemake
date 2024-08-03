#include "pch.h"
#include "HoraceRadiche.h"
#include "Object.h"
#include "SpriteSheet.h"
#include "OscillatingSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"
#include "Level.h"
#include "Bullet.h"
#include "BigBullet.h"
#include <iostream>
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

HoraceRadiche::HoraceRadiche()
	:m_Health{m_MaxHealth}
{
	std::string textPath{ "./resources/images/Levels/BotanicPanic/Horace_Radiche/" };

	m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + "intro.png", 6, 5 ,30 };
	m_pSpriteSheets[State::Attacking] = new SpriteSheet{ textPath + "Attack.png", 4, 1, 4};
	m_pSpriteSheets[State::Dying] = new SpriteSheet{ textPath + "death.png", 8, 2, 12 };

	m_HitBox = Rectf{ 400, 95, m_pSpriteSheets[State::Attacking]->GetFrameWidth()*0.6f,  m_pSpriteSheets[State::Attacking]->GetFrameHeight() * 0.6f };
	m_pTarget = std::make_shared<Point2f>(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 });
}

HoraceRadiche::~HoraceRadiche()
{
	for (std::pair<State, SpriteSheet*> pair : m_pSpriteSheets)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSpriteSheets.clear();
}

void HoraceRadiche::Draw()
{
	Point2f middle{ m_HitBox.left + m_HitBox.width / 2.f, m_HitBox.bottom };
	m_pSpriteSheets[m_State]->DrawBottom(middle);
	if (m_Hit)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.5f });
		m_pSpriteSheets[m_State]->DrawBottom(middle, true, 1);
	}
}

void HoraceRadiche::Update(float elapsedSec, const Level* level, const Rectf& player, std::vector<std::weak_ptr<Point2f>>& targets)
{
	m_pSpriteSheets[m_State]->Update(elapsedSec);
	switch (m_State)
	{
	case HoraceRadiche::State::Intro:
		UpdateIntro(elapsedSec);
		if (m_State != State::Intro)targets.push_back(m_pTarget);
		break;
	case HoraceRadiche::State::Attacking:
		UpdateAttack(elapsedSec, player);
		break;
	case HoraceRadiche::State::Dying:
		UpdateDeath(elapsedSec);
		break;
	default:
		break;
	}

	if (m_Hit)
	{
		m_HitTimer += elapsedSec;
		if (m_HitTimer >= m_HitTime)
		{
			m_HitTimer = 0;
			m_Hit = false;
		}
	}

	if (m_State != State::Dying && m_Health <= 0)
	{
		m_pTarget.reset();
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Horace_Radiche_Death);
		m_State = State::Dying;
	}
}

bool HoraceRadiche::GetActivated()
{
	return m_Activated;
}

bool HoraceRadiche::CeckHits(Rectf& shape)
{
	if (m_Activated && m_State != State::Dying)
	{
		bool hit{ false };

		if (utils::IsOverlapping(shape, m_HitBox))
		{
			hit = true;
		}
		return hit;
	}
	else return false;
}

float HoraceRadiche::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	if (m_Activated && m_State != State::Dying)
	{
		float damage{};
		for (int i{}; i < bullets.size(); i++)
		{
			if (bullets[i]->IsActivated())
			{
				if (utils::IsOverlapping(m_HitBox, bullets[i]->GetHitbox()))
				{
					m_Hit = true;
					m_Health -= bullets[i]->GetDamage();
					bullets[i]->Hit();
					damage += bullets[i]->GetDamage();
				}
			}

		}

		for (int i{}; i < bigBullets.size(); i++)
		{
			if (bigBullets[i]->IsActivated())
			{
				if (utils::IsOverlapping(m_HitBox, bigBullets[i]->GetHitbox()))
				{
					if (bigBullets[i]->Hit())
					{
						m_Hit = true;
						m_Health -= bigBullets[i]->GetDamage();
					}
				}
			}
		}
		return damage;
	}
	else return 0;
}

void HoraceRadiche::UpdateIntro(float elapsedSec)
{
	if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())m_State = State::Attacking;
}

void HoraceRadiche::UpdateAttack(float elapsedSec, const Rectf& player)
{
	float xValue{ m_HitBox.left + m_HitBox.width / 2 }, playerxValue{player.left + player.width/2};

	if (xValue >= playerxValue)m_Speed -= m_Acceleration * elapsedSec;
	else m_Speed += m_Acceleration * elapsedSec;

	if (m_Speed >= m_MaxSpeed)m_Speed = m_MaxSpeed;
	else if (m_Speed <= -m_MaxSpeed)m_Speed = -m_MaxSpeed;

	m_HitBox.left += m_Speed * elapsedSec;
	m_pTarget->x = m_HitBox.left + m_HitBox.width / 2;
}

void HoraceRadiche::UpdateDeath(float elapsedSec)
{
	m_DeathTimer += elapsedSec;

	if (m_DeathTimer >= m_DeathTime)
	{
		m_Activated = false;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Horace_Radiche_Explode);
		g_pSFXManager->CreateParticle(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 }, ParticleSoundeffectManager::ParticleType::Horace_Radiche_Smoke);
		g_pSFXManager->CreateParticle(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + 2*m_HitBox.height/3}, ParticleSoundeffectManager::ParticleType::Horace_Radiche_Leaves);
	}
}
