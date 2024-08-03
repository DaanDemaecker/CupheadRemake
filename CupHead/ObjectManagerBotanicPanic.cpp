#include "pch.h"
#include "ObjectManagerBotanicPanic.h"
#include "SalSpudder.h"
#include "OllieBulb.h"
#include "ChaunceyChantenay.h"
#include "HoraceRadiche.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"

extern ParticleSoundeffectManager* g_pSFXManager;

ObjectManagerBotanicPanic::ObjectManagerBotanicPanic()
{
	m_pSalSpudder = new SalSpudder{};
	m_pOllieBulb = new OllieBulb{};
	m_pChauncyChantenay = new ChaunceyChantenay{};
	m_pHoraceRadiche = new HoraceRadiche{};
}

ObjectManagerBotanicPanic::~ObjectManagerBotanicPanic()
{
	delete m_pSalSpudder;
	m_pSalSpudder = nullptr;
	delete m_pOllieBulb;
	m_pOllieBulb = nullptr;
	delete m_pChauncyChantenay;
	m_pChauncyChantenay = nullptr;
	delete m_pHoraceRadiche;
	m_pHoraceRadiche = nullptr;
}

void ObjectManagerBotanicPanic::DrawLevel1() const
{
	if (m_State == State::chaunceyChantenay)m_pChauncyChantenay->DrawCarrotIntro();
}

void ObjectManagerBotanicPanic::DrawLevel3() const
{
	if (m_State == State::chaunceyChantenay||m_State == State::knockout)m_pChauncyChantenay->Draw();
}

void ObjectManagerBotanicPanic::DrawLevel4() const
{
	if (m_RadishActive)m_pHoraceRadiche->Draw();

	switch (m_State)
	{
	case ObjectManagerBotanicPanic::State::salSpudder:
		m_pSalSpudder->Draw();
		break;
	case ObjectManagerBotanicPanic::State::ollieBulb:
		m_pOllieBulb->Draw();
		break;
	case ObjectManagerBotanicPanic::State::chaunceyChantenay:
		m_pChauncyChantenay->DrawObjects();
		break;
	default:
		break;
	}
}

void ObjectManagerBotanicPanic::Update(float elapsedSec, const Level* level, const Rectf& shape)
{
	switch (m_State)
	{
	case ObjectManagerBotanicPanic::State::salSpudder:
		m_pSalSpudder->Update(elapsedSec, level, m_pTargets);
		if (!m_pSalSpudder->GetActivated())
		{
			m_State = State::ollieBulb;
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Ollie_Bulb_Rising);
		}
		break;
	case ObjectManagerBotanicPanic::State::ollieBulb:
		m_pOllieBulb->Update(elapsedSec, level, m_pTargets);
		if (!m_pOllieBulb->GetActivated())
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Rise);
			m_State = State::chaunceyChantenay;
		}
		break;
	case ObjectManagerBotanicPanic::State::chaunceyChantenay:
		m_pChauncyChantenay->Update(elapsedSec, level, shape, m_pTargets);
		if (m_pChauncyChantenay->IsDead())m_State = State::knockout;
		break;
	case ObjectManagerBotanicPanic::State::knockout:
		m_pChauncyChantenay->UpdateDeath(elapsedSec);
		break;
	default:
		break;
	}

	if (m_RadishActive)
	{
		m_pHoraceRadiche->Update(elapsedSec, level, shape, m_pTargets);
		if (!m_pHoraceRadiche->GetActivated())
		{
			m_RadishActive = false;
			m_RadishDefeated = true;
		}
	}
	else if (m_State == State::ollieBulb && m_pOllieBulb->ActivateRadish())
	{
		m_RadishActive = true;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Horace_Radiche_Intro);
	}
}

bool ObjectManagerBotanicPanic::CheckParry(const Rectf& shape, float damage)
{
	switch (m_State)
	{
	case ObjectManagerBotanicPanic::State::salSpudder:
		return m_pSalSpudder->CheckParry(shape, damage);
		break;
	case ObjectManagerBotanicPanic::State::ollieBulb:
		return m_pOllieBulb->CheckParry(shape, damage);
		break;
	case ObjectManagerBotanicPanic::State::chaunceyChantenay:
		return m_pChauncyChantenay->CheckParry(shape, damage);
		break;
	case ObjectManagerBotanicPanic::State::knockout:
		break;
	}
	return false;
}

bool ObjectManagerBotanicPanic::Checkhits(Rectf& shape, bool parry, Vector2f& velocity)
{
	bool hit{ false };

	switch (m_State)
	{
	case ObjectManagerBotanicPanic::State::salSpudder:
		hit = m_pSalSpudder->CeckHits(shape, parry);
		break;
	case ObjectManagerBotanicPanic::State::ollieBulb:
		hit = m_pOllieBulb->CeckHits(shape, parry);
		break;
	case ObjectManagerBotanicPanic::State::chaunceyChantenay:
		hit = m_pChauncyChantenay->CeckHits(shape, parry);
		break;
	case ObjectManagerBotanicPanic::State::knockout:
		break;
	}

	if (m_RadishActive && !hit)hit = m_pHoraceRadiche->CeckHits(shape);

	if (m_State == State::knockout) hit = false;

	return hit;
}

float ObjectManagerBotanicPanic::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	float damage{};

	switch (m_State)
	{
	case ObjectManagerBotanicPanic::State::salSpudder:
		damage += m_pSalSpudder->CheckBullets(bullets, bigBullets);
		break;
	case ObjectManagerBotanicPanic::State::ollieBulb:
		damage += m_pOllieBulb->CheckBullets(bullets, bigBullets);
		break;
	case ObjectManagerBotanicPanic::State::chaunceyChantenay:
		damage += m_pChauncyChantenay->CheckBullets(bullets, bigBullets);
		break;
	case ObjectManagerBotanicPanic::State::knockout:
		break;
	}

	if (m_RadishActive)damage += m_pHoraceRadiche->CheckBullets(bullets, bigBullets);

	return damage;
}

bool ObjectManagerBotanicPanic::IsGameOver()
{
	if (m_State == State::knockout)return true;
	else return false;
}

void ObjectManagerBotanicPanic::FixParallax(const Point2f& cameraDifference)
{
	m_pChauncyChantenay->FixCameraMove(cameraDifference.x);
}

LoseScreenData ObjectManagerBotanicPanic::GetLoseScreenData()
{
	LoseScreenData data{};

	if (m_RadishActive || m_RadishDefeated)
	{
		data = LoseScreenData{ true, "./resources/images/Levels/BotanicPanic/Chauncey_Chantenay/Death_Card.png" };
	}
	else
	{
		switch (m_State)
		{
		case ObjectManagerBotanicPanic::State::salSpudder:
			data.textPath = "./resources/images/Levels/BotanicPanic/Sal_Spudder/Death_Card.png";
			data.distance = 1 - m_pSalSpudder->GetHealth() / m_pSalSpudder->GetMaxHealth();
			break;
		case ObjectManagerBotanicPanic::State::ollieBulb:
			data.textPath = "./resources/images/Levels/BotanicPanic/Ollie_Bulb/Death_Card.png";
			data.distance = 2 - m_pOllieBulb->GetHealth() / m_pOllieBulb->GetMaxHealth();
			break;
		case ObjectManagerBotanicPanic::State::chaunceyChantenay:
			data.textPath = "./resources/images/Levels/BotanicPanic/Chauncey_Chantenay/Death_Card.png";
			data.distance = 3 - m_pChauncyChantenay->GetHealth() / m_pOllieBulb->GetMaxHealth();
			break;
		}
		data.nodes = new float[] {1.f / 3.f, 2 * 1 / 3.f};
		data.nodeAmount = 2;
		data.secret = false;
	}
	return data;
}
