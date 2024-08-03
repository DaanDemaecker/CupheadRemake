#include "pch.h"
#include "SalSpudder.h"
#include "Object.h"
#include "SpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "OscillatingSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "utils.h"
#include "Level.h"
#include "Bullet.h"
#include "BigBullet.h"
#include "ParticleSoundeffectManager.h"
#include <iostream>
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

SalSpudder::SalSpudder()
	:m_Health{ m_MaxHealth }
{
	std::string textPath{ "./resources/images/Levels/BotanicPanic/Sal_Spudder/" };
	m_pGroundBreakingTexture = new LoopSpriteSheet{ textPath + "Ground_Breaking.png", 5, 4, 18, 17, 17 ,1/10.f };

	m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + "intro.png", 6, 2 ,9 };
	m_pSpriteSheets[State::Idle] = new OscillatingSpriteSheet{ textPath + "idle.png", 5, 1, 5 };
	m_pSpriteSheets[State::Attacking] = new OneCycleSpriteSheet{ textPath + "Attack.png", 6, 3, 18, 1/20.f };
	m_pSpriteSheets[State::Dying] = new OscillatingSpriteSheet{ textPath + "death.png", 6, 1, 6 };

	m_HitBox = Rectf{ 800, 70, m_pSpriteSheets[State::Intro]->GetFrameWidth()/2.4f, m_pSpriteSheets[State::Intro]->GetFrameWidth()/1.7f};
	m_pTarget = std::make_shared<Point2f>(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 });
	m_ObjectSpawn = Point2f{ m_HitBox.left-80, m_HitBox.bottom +75 };

	m_pObjects.push_back(new Object{ textPath + "DirtBall.png", ParticleSoundeffectManager::ParticleType::DirtBall_Death, ParticleSoundeffectManager::SFXType::Sal_Spudder_Object_Death ,8, 0, true });
	m_pObjects.push_back(new Object{ textPath + "DirtBall.png", ParticleSoundeffectManager::ParticleType::DirtBall_Death, ParticleSoundeffectManager::SFXType::Sal_Spudder_Object_Death, 8, 0, true });
	m_pObjects.push_back(new Object{ textPath + "DirtBall.png", ParticleSoundeffectManager::ParticleType::DirtBall_Death, ParticleSoundeffectManager::SFXType::Sal_Spudder_Object_Death, 8, 0, true });
	m_pObjects.push_back(new Object{ textPath + "Worm.png", ParticleSoundeffectManager::ParticleType::Worm_Death, ParticleSoundeffectManager::SFXType::Sal_Spudder_Object_Death, 4, 0, true, true, true });

}

SalSpudder::~SalSpudder()
{
	delete m_pGroundBreakingTexture;
	m_pGroundBreakingTexture = nullptr;
	for (std::pair<State, SpriteSheet*> pair : m_pSpriteSheets)
	{
		delete pair.second;
		pair.second;
	}
	m_pSpriteSheets.clear();

	for (int i{}; i < m_pObjects.size(); i++)
	{
		delete m_pObjects[i];
		m_pObjects[i] = nullptr;
	}
	m_pSpriteSheets.clear();
}

void SalSpudder::Draw()
{
	if (m_GroundBroken && !m_IsBackInGround)
	{
		if (m_State != State::Dying)
		{
			m_pSpriteSheets[m_State]->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2.f, m_HitBox.bottom }, true);
			if (m_Hit)
			{
				utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.5f });
				m_pSpriteSheets[m_State]->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2.f, m_HitBox.bottom }, true, 1);
			}
		}
		else m_pSpriteSheets[m_State]->DrawPercentage(Point2f{ m_HitBox.left + m_HitBox.width / 2.f, m_HitBox.bottom}, m_Percentage);
	}

	if (!m_IsBackInGround)m_pGroundBreakingTexture->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom - 30 });
	else
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, m_Percentage });
		m_pGroundBreakingTexture->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom - 30 }, true, 2);
	}

	for (int i{}; i < m_pObjects.size(); i++)
	{
		m_pObjects[i]->Draw();
	}
}

void SalSpudder::Update(float elapsedSec, const Level* level, std::vector<std::weak_ptr<Point2f>>& targets)
{
	m_pGroundBreakingTexture->Update(elapsedSec);
	if (!m_GroundBroken && m_pGroundBreakingTexture->GetCrFrame() >= 8)
	{
		m_GroundBroken = true;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Sal_Spudder_Rising);
	}

	if (m_GroundBroken)
	{
		m_pSpriteSheets[m_State]->Update(elapsedSec);
		switch (m_State)
		{
		case SalSpudder::State::Intro:
			UpdateIntro(elapsedSec);
			if (m_State != State::Intro)
			{
				targets.push_back(m_pTarget);
			}
			break;
		case SalSpudder::State::Idle:
			UpdateIdle(elapsedSec);
			break;
		case SalSpudder::State::Attacking:
			UpdateAttack(elapsedSec);
			break;
		case SalSpudder::State::Dying:
			UpdateDeath(elapsedSec);
			break;
		default:
			break;
		}

		for (int i{}; i < m_pObjects.size(); i++)
		{
			m_pObjects[i]->Update(elapsedSec, level);
		}

		if (m_State != State::Dying && m_Health <= 0)
		{
			m_pTarget.reset();
			m_State = State::Dying;
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Boss_Death);
		}
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

}

bool SalSpudder::GetActivated()
{
	return m_Activated;
}

void SalSpudder::UpdateIntro(float elapsedSec)
{

	if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->GetFinishedCycle())
	{
		static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->ResetCycle();
		m_State = State::Idle;
	}
}

void SalSpudder::UpdateIdle(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= 3.f)
	{
		m_State = State::Attacking;
		m_Timer = 0;
	}
}

void SalSpudder::UpdateAttack(float elapsedSec)
{
	if (m_pSpriteSheets[State::Attacking]->GetCrFrame() == 10 )
	{
		if (!m_pObjects[m_Attack]->GetActivated())
		{
			g_pSFXManager->CreateParticle(m_ObjectSpawn, ParticleSoundeffectManager::ParticleType::Potato_Spit);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Sal_Spudder_Spit);
			m_pObjects[m_Attack]->SetObject(m_ObjectSpawn, float(M_PI), m_ObjectSpeed);
		}
	}


	if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Attacking])->GetFinishedCycle())
	{
		static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Attacking])->ResetCycle();
		if (m_Attack <= 2)
		{
			m_Attack++;
		}
		else
		{
			m_Attack = 0;
			m_State = State::Idle;
		}
	}
}

void SalSpudder::UpdateDeath(float elapsedSec)
{
	if (m_DeathTimer < m_DeathTime)
	{
		m_DeathTimer += elapsedSec;
	}
	else
	{
		if (!m_IsBackInGround)
		{
			if (m_Percentage > 0.01f)
			{
				m_Percentage -= elapsedSec / 3;
			}
			else
			{
				m_IsBackInGround = true;
				m_Percentage = 1.f;
			}
		}
		else
		{
			if (m_Percentage > 0.01f)
			{
				m_Percentage -= elapsedSec / 1.5f;
			}
			else
			{
				m_Activated = false;
			}
		}
	}

	if (!m_IsBackInGround)
	{
		m_DeathEffectTimer += elapsedSec;
		if (m_DeathEffectTimer >= m_DeathEffectTime)
		{
			m_DeathEffectTimer -= m_DeathEffectTime;
			Point2f pos{};
			pos.x = rand() % int(m_HitBox.width) + m_HitBox.left;
			pos.y = rand() % int(m_HitBox.height) + m_HitBox.bottom;
			g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Boss_Explosion);
		}
	}
}

bool SalSpudder::CheckParry(const Rectf& shape, float damage)
{
	int i{};
	bool hit{ false };
	while (!hit && i < m_pObjects.size())
	{
		hit = m_pObjects[i]->CheckParry(shape);
		i++;
	}
	if (hit)m_Health -= damage;

	return hit;
}

bool SalSpudder::CeckHits(Rectf& shape, bool parry)
{
	if (m_Activated && m_State != State::Dying)
	{
		int i{};
		bool hit{ false };
		while (!hit && i < m_pObjects.size())
		{
			hit = m_pObjects[i]->CeckHit(shape, parry);
			i++;
		}

		if (utils::IsOverlapping(shape, m_HitBox))
		{
			hit = true;
		}
		return hit;
	}
	else return false;
}

float SalSpudder::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
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

float SalSpudder::GetHealth()
{
	return m_Health;
}

float SalSpudder::GetMaxHealth()
{
	return m_MaxHealth;
}
