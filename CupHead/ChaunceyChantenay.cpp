#include "pch.h"
#include "ChaunceyChantenay.h"
#include "Object.h"
#include "SpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "OscillatingSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "ParticleSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"
#include "Level.h"
#include "Bullet.h"
#include "BigBullet.h"
#include "Carrot.h"
#include <iostream>
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

ChaunceyChantenay::ChaunceyChantenay()
{
	m_Health = m_MaxHealth;

	std::string textPath{ "./resources/images/Levels/BotanicPanic/Chauncey_Chantenay/" };
	m_pGroundBreakingTexture = new LoopSpriteSheet{ textPath + "Ground_Breaking.png", 5, 4, 18, 17, 17 ,1 / 10.f };
	m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + "intro.png", 8, 4 ,19 };
	m_pSpriteSheets[State::Idle] = new SpriteSheet{ textPath + "idle.png", 10, 3, 22 };
	m_pSpriteSheets[State::Attacking] = new LoopSpriteSheet{ textPath + "Attack.png", 10, 2, 14, 11, 13, 1 / 10.f };
	m_pSpriteSheets[State::Dying] = new OscillatingSpriteSheet{ textPath + "death.png", 13, 1, 13 };

	for(int i{}; i< m_BeamAttackAmount; i++)m_pObjects.push_back(new Object{ textPath + "BeamAttack.png",ParticleSoundeffectManager::ParticleType::Null,ParticleSoundeffectManager::SFXType::None , 7, 3, true });

	m_Pos = Point2f{ 600, 170 };
	m_HitboxCorner = Point2f{ m_Pos.x - 80, m_Pos.y + m_pSpriteSheets[State::Attacking]->GetFrameHeight() - 250 };
	m_HitBox = Rectf{ m_HitboxCorner.x, m_HitboxCorner.y, 160, 160 };
	m_pTarget = std::make_shared<Point2f>(Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 });

	m_ObjectSpawn = Point2f{ m_HitBox.left + m_HitBox.width / 2, m_HitBox.bottom + m_HitBox.height / 2 };
	m_TranslatedObjectSpawn = m_ObjectSpawn;

	m_pBeamChargeTexture = new Texture{ textPath + "Beam_Charge.png" };
	m_pBeamChargeSpriteSheet = new ParticleSpriteSheet{m_pBeamChargeTexture, 4, 4, 16, m_ObjectSpawn };
}

ChaunceyChantenay::~ChaunceyChantenay()
{
	delete m_pGroundBreakingTexture;
	m_pGroundBreakingTexture = nullptr;
	delete m_pBeamChargeSpriteSheet;
	m_pBeamChargeSpriteSheet = nullptr;
	delete m_pBeamChargeTexture;
	m_pBeamChargeTexture = nullptr;

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

	for (int i{}; i < m_pCarrots.size(); i++)
	{
		delete m_pCarrots[i];
		m_pCarrots[i] = nullptr;
	}
	m_pCarrots.clear();
}

void ChaunceyChantenay::DrawCarrotIntro() const
{
	for (int i{}; i < m_pCarrots.size(); i++)
	{
		m_pCarrots[i]->DrawIntro();
	}
}

void ChaunceyChantenay::Draw()
{
	if (m_GroundBroken)
	{
		m_pSpriteSheets[m_State]->DrawBottom(m_Pos);
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 0.5f });
		if(m_Hit && m_State != State::Dying)m_pSpriteSheets[m_State]->DrawBottom(m_Pos, true, 1);
	}
	m_pGroundBreakingTexture->DrawBottom(Point2f{ m_Pos.x, m_Pos.y - 30 });
	if (m_ChargingBeam && m_State != State::Dying)m_pBeamChargeSpriteSheet->Draw();
}

void ChaunceyChantenay::DrawObjects() const
{
	for (int i{}; i < m_pObjects.size(); i++)
	{
		m_pObjects[i]->Draw();
	}
	for (int i{}; i < m_pCarrots.size(); i++)
	{
		m_pCarrots[i]->Draw();
	}
}

void ChaunceyChantenay::Update(float elapsedSec, const Level* level, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets)
{
	m_pGroundBreakingTexture->Update(elapsedSec);
	if (m_ChargingBeam)m_pBeamChargeSpriteSheet->Update(elapsedSec);
	if (!m_GroundBroken && m_pGroundBreakingTexture->GetCrFrame() >= 8)m_GroundBroken = true;

	if (m_GroundBroken)
	{
		m_pSpriteSheets[m_State]->Update(elapsedSec);
		switch (m_State)
		{
		case ChaunceyChantenay::State::Intro:
			UpdateIntro(elapsedSec);
			if (m_State != State::Intro)targets.push_back(m_pTarget);
			break;
		case ChaunceyChantenay::State::Idle:
			UpdateIdle(elapsedSec);
			break;
		case ChaunceyChantenay::State::Attacking:
			UpdateAttack(elapsedSec, shape);
			break;
		case ChaunceyChantenay::State::Dying:
			UpdateDeath(elapsedSec);
			break;
		default:
			break;
		}
		UpdateObjects(elapsedSec, level, shape, targets);
	}

	if (m_State != State::Dying && m_Health <= 0)
	{
		m_pTarget.reset();
		g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Attack);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Boss_Death);
		m_State = State::Dying;
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

void ChaunceyChantenay::FixCameraMove(float cameraDistance)
{
	m_TranslatedObjectSpawn.x = m_ObjectSpawn.x + cameraDistance;
	m_HitBox.left = m_HitboxCorner.x + cameraDistance;
	if(m_State != State::Dying)m_pTarget->x = m_HitBox.left + m_HitBox.width/2;
}

bool ChaunceyChantenay::IsDead()
{
	if (m_State == State::Dying)return true;
	else return false;
}

void ChaunceyChantenay::UpdateIntro(float elapsedSec)
{
	if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->GetFinishedCycle())
	{
		static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[State::Intro])->ResetCycle();
		m_State = State::Idle;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Attack, 5);
	}
}

void ChaunceyChantenay::UpdateIdle(float elapsedSec)
{
	m_CarrotTimer += elapsedSec;
	if (m_CarrotTimer >= 2.f)
	{
		m_CarrotTimer = 0;
		m_pCarrots.push_back(new Carrot{});
		if (m_CarrotNumber++ == 4)
		{
			m_CarrotNumber = 0;
			m_State = State::Attacking;	
			g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Attack);
		}
	}
}

void ChaunceyChantenay::UpdateAttack(float elapsedSec, const Rectf& shape)
{
	if (static_cast<LoopSpriteSheet*>(m_pSpriteSheets[State::Attacking])->IsLooping() && !m_ChargingBeam && !m_DoingBeamAttack && !m_BeamAttackDone)
	{
		m_ChargingBeam = true;
	}

	if (m_ChargingBeam && m_pBeamChargeSpriteSheet->GetFinishedCycle())
	{
		
		m_pBeamChargeSpriteSheet->ResetCycle();
		m_ChargingBeam = false;
		m_DoingBeamAttack = true;
		m_BeamAttackAngle = atan2f(shape.bottom + shape.height / 2 - m_ObjectSpawn.y, shape.left + shape.width / 2 - m_ObjectSpawn.x);
	}


	if (m_BeamAttackDone)
	{
		m_BeamAttackDone = false;
		static_cast<LoopSpriteSheet*>(m_pSpriteSheets[State::Attacking])->ResetCycle();
		if (++m_Attack == 3)
		{
			m_Attack = 0;
			m_State = State::Idle;
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Attack, 5);
		}
	}
}

void ChaunceyChantenay::UpdateDeath(float elapsedSec)
{
	m_pSpriteSheets[State::Dying]->Update(elapsedSec);

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

void ChaunceyChantenay::UpdateObjects(float elapsedSec, const Level* level, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets)
{
	DeleteInactiveObjects();

	for (int i{}; i < m_pObjects.size(); i++)
	{
		m_pObjects[i]->Update(elapsedSec, level);
	}
	for (int i{}; i < m_pCarrots.size(); i++)
	{
		m_pCarrots[i]->Update(elapsedSec, shape, targets);
	}
	if (m_DoingBeamAttack)UpdateBeamAttack(elapsedSec);
}

void ChaunceyChantenay::DeleteInactiveObjects()
{
	int carrotAmount{ int(m_pCarrots.size()) };
	for (int i{}; i < carrotAmount; i++)
	{
		if (!m_pCarrots[i]->GetActivated())
		{
			delete m_pCarrots[i];
			m_pCarrots[i] = nullptr;
			if (carrotAmount-- > 0)
			{
				m_pCarrots[i] = m_pCarrots[m_pCarrots.size() - 1];
				m_pCarrots.pop_back();
				i--;
			}
		}
	}
}

void ChaunceyChantenay::UpdateBeamAttack(float elapsedSec)
{
	m_BeamAttackTimer += elapsedSec;
	
	if (m_BeamAttackTimer >= m_BeamAttackDelay)
	{
		m_BeamAttackTimer -= m_BeamAttackDelay;
		m_pObjects[m_BeamAttackCurrent]->SetObject(m_TranslatedObjectSpawn, m_BeamAttackAngle, m_ObjectSpeed);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Beam);
		if (++m_BeamAttackCurrent >= m_BeamAttackAmount - 1)
		{
			m_BeamAttackDone = true;
			m_BeamAttackCurrent = 0;
			m_DoingBeamAttack = false;
			m_BeamAttackTimer = 0;
		}
	}
}

bool ChaunceyChantenay::CheckParry(const Rectf& shape, float damage)
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

bool ChaunceyChantenay::CeckHits(Rectf& shape, bool parry)
{
	if (m_State != State::Dying)
	{
		int i{};
		bool hit{ false };
		while (!hit && i < m_pObjects.size())
		{
			hit = m_pObjects[i]->CeckHit(shape, parry);
			i++;
		}

		if (!hit)
		{
			i = 0;
			while (!hit && i < m_pCarrots.size())
			{
				hit = m_pCarrots[i]->CheckHit(shape);
				i++;
			}
		}
		return hit;
	}
	else return false;
}

float ChaunceyChantenay::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	if (m_State != State::Dying)
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

				for (int j{}; j < m_pCarrots.size(); j++)
				{
					if (m_pCarrots[j]->CheckBulletHit(bullets[i]->GetHitbox()))
					{
						bullets[i]->Hit();
						damage += bullets[i]->GetDamage();
					}
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
				else
				{
					for (int j{}; j < m_pCarrots.size(); j++)
					{
						if (m_pCarrots[j]->CheckBulletHit(bigBullets[i]->GetHitbox()))
						{
							bigBullets[i]->Hit();
						}
					}
				}
			}
		}
		return damage;
	}
	else return 0;
}

float ChaunceyChantenay::GetHealth()
{
	return m_Health;
}

float ChaunceyChantenay::GetMaxHealth()
{
	return m_MaxHealth;
}
