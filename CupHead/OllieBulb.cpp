#include "pch.h"
#include "OllieBulb.h"
#include "Object.h"
#include "SpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "OscillatingSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "utils.h"
#include "Level.h"
#include "Bullet.h"
#include "BigBullet.h"
#include <iostream>
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

OllieBulb::OllieBulb()
	:m_Health{ m_MaxHealth }
{
	std::string textPath{ "./resources/images/Levels/BotanicPanic/Ollie_Bulb/" };
	m_pGroundBreakingTexture = new LoopSpriteSheet{ textPath + "Ground_Breaking.png", 5, 4, 18, 17, 17 ,1 / 10.f };
	m_pCryingLoopTexture = new LoopSpriteSheet{ textPath + "Crying_Loop.png", 4, 4, 16, 5, 7 };

	m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + "intro.png", 5, 5 ,24 };
	m_pSpriteSheets[State::Idle] = new OscillatingSpriteSheet{ textPath + "idle.png", 5, 3, 15 };
	m_pSpriteSheets[State::Attacking] = new LoopSpriteSheet{ textPath + "Attack.png", 5, 5, 24, 15, 20 ,1 / 10.f };
	m_pSpriteSheets[State::Dying] = new OscillatingSpriteSheet{ textPath + "death.png", 6, 1, 6 };
	m_pSpriteSheets[State::Happy] = new LoopSpriteSheet{ textPath + "Happy.png", 5, 3, 13, 7, 12 };

	m_HitBox = Rectf{ 430, 70, m_pSpriteSheets[State::Intro]->GetFrameWidth() / 1.2f, m_pSpriteSheets[State::Intro]->GetFrameWidth() / 1.5f };
	m_pTarget = std::make_shared<Point2f>(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 });

	m_pObjects.push_back(new Object{ textPath + "Drop_Np.png", ParticleSoundeffectManager::ParticleType::Tear_Splash, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true });
	m_pObjects.push_back(new Object{ textPath + "Drop_Np.png", ParticleSoundeffectManager::ParticleType::Tear_Splash, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true });
	m_pObjects.push_back(new Object{ textPath + "Drop_P.png", ParticleSoundeffectManager::ParticleType::Tear_Splash_Pink, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true, true, true });
	m_pObjects.push_back(new Object{ textPath + "Drop_Np.png", ParticleSoundeffectManager::ParticleType::Tear_Splash, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true });
	m_pObjects.push_back(new Object{ textPath + "Drop_Np.png", ParticleSoundeffectManager::ParticleType::Tear_Splash, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true });
	m_pObjects.push_back(new Object{ textPath + "Drop_P.png", ParticleSoundeffectManager::ParticleType::Tear_Splash_Pink, ParticleSoundeffectManager::SFXType::Ollie_Bulb_Tear_Death, 3, 0, true, true, true });

}

OllieBulb::~OllieBulb()
{
	delete m_pGroundBreakingTexture;
	m_pGroundBreakingTexture = nullptr;
	delete m_pCryingLoopTexture;
	m_pCryingLoopTexture = nullptr;

	for (std::pair<State, SpriteSheet*> pair : m_pSpriteSheets)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSpriteSheets.clear();

	for (int i{}; i < m_pObjects.size(); i++)
	{
		delete m_pObjects[i];
		m_pObjects[i] = nullptr;
	}
	m_pObjects.clear();
}

void OllieBulb::Draw()
{
	Point2f middle{ m_HitBox.left + m_HitBox.width / 2.f, m_HitBox.bottom };

	if (m_GroundBroken && !m_IsBackInGround)
	{
		if (m_State != State::Dying && m_State != State::Happy)
		{
			m_pSpriteSheets[m_State]->DrawBottom(middle);
			if (m_Hit)
			{
				utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.5f });
				m_pSpriteSheets[m_State]->DrawBottom(middle, true, 1);
			}
		}
		else m_pSpriteSheets[m_State]->DrawPercentage(middle, m_Percentage);
	}
	if (!m_IsBackInGround)m_pGroundBreakingTexture->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom - 60 });
	else
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, m_Percentage });
		m_pGroundBreakingTexture->DrawBottom(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom - 60 }, true, 2);
	}

	if (m_Crying)
	{
		m_pCryingLoopTexture->DrawBottom(Point2f{ middle.x + 345, middle.y + 345 });
		m_pCryingLoopTexture->DrawBottom(Point2f{ middle.x - 350, middle.y + 345 }, false);
	}

	for (int i{}; i < m_pObjects.size(); i++)
	{
		m_pObjects[i]->Draw();
	}
}

void OllieBulb::Update(float elapsedSec, const Level* level, std::vector<std::weak_ptr<Point2f>>& targets)
{
	m_pGroundBreakingTexture->Update(elapsedSec);
	if (!m_GroundBroken && m_pGroundBreakingTexture->GetCrFrame() >= 8)
	{
		m_GroundBroken = true;
		
	}

	if (m_GroundBroken)
	{
		m_pSpriteSheets[m_State]->Update(elapsedSec);
		switch (m_State)
		{
		case State::Intro:
			UpdateIntro(elapsedSec);
			if (m_State != State::Intro)targets.push_back(m_pTarget);
			break;
		case State::Idle:
			UpdateIdle(elapsedSec);
			break;
		case State::Attacking:
			UpdateAttack(elapsedSec);
			break;
		case State::Dying:
			UpdateDeath(elapsedSec);
			break;
		case State::Happy:
			UpdateHappy(elapsedSec);
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
			g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Ollie_Bulb_Crying);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Boss_Death);
			m_State = State::Dying;
			m_Crying = false;
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

	if (m_Health >= m_MaxHealth && m_State != State::Happy)
	{
		m_HappyTimer += elapsedSec;
		if (m_HappyTimer >= m_HappyTime)
		{
			m_pTarget.reset();
			m_State = State::Happy;
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Ollie_Bulb_Happy);
		}
	}
}

bool OllieBulb::GetActivated()
{
	return m_Activated;
}

bool OllieBulb::ActivateRadish()
{
	if (m_State == State::Happy && m_IsBackInGround)return true;
	else return false;
}

void OllieBulb::UpdateIntro(float elapsedSec)
{

	if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->GetFinishedCycle())
	{
		static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->ResetCycle();
		m_State = State::Idle;
	}
}

void OllieBulb::UpdateIdle(float elapsedSec)
{
	m_Timer += elapsedSec;
	if (m_Timer >= 1.f && m_Health < m_MaxHealth)
	{
		
		m_State = State::Attacking;
		m_Timer = 0;
	}
}

void OllieBulb::UpdateAttack(float elapsedSec)
{
	m_AttackTimer += elapsedSec;
	if (m_AttackTimer >= 5.f)
	{
		m_AttackTimer = 0;
		static_cast<LoopSpriteSheet*>(m_pSpriteSheets[State::Attacking])->StopLoop();
		m_pCryingLoopTexture->StopLoop();
	}


	if (m_Crying)
	{
		m_pCryingLoopTexture->Update(elapsedSec);
		m_ObjectTimer += elapsedSec;
		if(m_ObjectTimer >= m_ObjectDelay)
		{
			NewObject();
			m_ObjectDelay = float(rand() % 20 - 5) / 10;
			m_ObjectTimer = 0;
		}
	}


	if (!m_Crying && m_pSpriteSheets[State::Attacking]->GetCrFrame() == 14)
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Ollie_Bulb_Crying);
		m_Crying = true;
	}

	if (m_pCryingLoopTexture->GetCrFrame() >= 15)
	{
		g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Ollie_Bulb_Crying);
		m_pCryingLoopTexture->ResetCycle();
		m_Crying = false;
	}
}

void OllieBulb::UpdateDeath(float elapsedSec)
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

void OllieBulb::UpdateHappy(float elapsedSec)
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
}

bool OllieBulb::CheckParry(const Rectf& shape, float damage)
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

bool OllieBulb::CeckHits(Rectf& shape, bool parry)
{
	if (m_Activated && m_State != State::Dying && m_State != State::Intro && m_State != State::Happy)
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

float OllieBulb::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	if (m_Activated && m_State != State::Dying && m_State != State::Happy)
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

float OllieBulb::GetHealth()
{
	return m_Health;
}

float OllieBulb::GetMaxHealth()
{
	return m_MaxHealth;
}

void OllieBulb::NewObject()
{
	if (!m_pObjects[m_CurrentObject]->GetActivated())
	{
		Point2f point{};

		bool right{ bool(rand() % 2) };

		if (right)
		{
			point = Point2f{ float(rand() % int(m_HitBox.left) + m_HitBox.left + m_HitBox.width),720 };
		}
		else 
		{
			point = Point2f{ float(rand() % int(m_HitBox.left)),720 };
		}
		
		m_pObjects[m_CurrentObject++]->SetObject(point, float(3 * M_PI / 2.f), m_ObjectSpeed);
		m_CurrentObject %= m_pObjects.size();
	}
}