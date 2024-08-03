#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "BigBullet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include "ChargeBulletManager.h"
#include "Texture.h"
#include "PeaShooter.h"
#include "SpreadShot.h"
#include "Roundabout.h"
#include "Lobber.h"
#include "Charge.h"
#include "Chaser.h"
#include "BigPeaShooter.h"
#include "BigSpreadShot.h"
#include "BigRoundabout.h"
#include "BigLobber.h"
#include "BigCharge.h"
#include "BigChaser.h"
#include "ObjectManager.h"
#include <iostream>
#include <algorithm>

extern ParticleSoundeffectManager* g_pSFXManager;


BulletManager::BulletManager(float damageMultiplier, Enums::BulletType bullet1Type)
	:m_DamageMultiplier{damageMultiplier}, m_CurrentBulletType{bullet1Type}
{
	m_FireRate = Bullet::GetFireRate(m_CurrentBulletType);

	std::string textPath{ "./resources/Images/Player/Bullets/" };

	m_pSmallBulletTextures[Enums::BulletType::PeaShooter] = new Texture{ textPath + "Peashooter/Peashooter_Loop.png" };
	m_pSmallBulletTextures[Enums::BulletType::SpreadShot] = new Texture{ textPath + "SpreadShot/SpreadShot_Loop.png" };
	m_pSmallBulletTextures[Enums::BulletType::Roundabout] = new Texture{ textPath + "Roundabout/Roundabout_Loop.png" };
	m_pSmallBulletTextures[Enums::BulletType::Lobber] = new Texture{ textPath + "Lobber/Lobber_Loop.png" };
	m_pSmallBulletTextures[Enums::BulletType::Charge] = new Texture{ textPath + "Charge/Charge_Big_Loop.png" };
	m_pSmallBulletTextures[Enums::BulletType::Chaser] = new Texture{ textPath + "Chaser/Chaser_Loop.png" };

	m_pSmallChargeTexture = new Texture{ textPath + "Charge/Charge_Small_Loop.png" };

	m_pBigBulletTextures[Enums::BulletType::PeaShooter] = new Texture{ textPath + "Peashooter/Peashooter_Special_Loop.png" };
	m_pBigBulletTextures[Enums::BulletType::SpreadShot] = new Texture{ textPath + "SpreadShot/SpreadShot_Special_Loop.png" };
	m_pBigBulletTextures[Enums::BulletType::Roundabout] = new Texture{ textPath + "Roundabout/Roundabout_Special_Loop.png" };
	m_pBigBulletTextures[Enums::BulletType::Lobber] = new Texture{ textPath + "Lobber/Lobber_Special_Loop.png" };
	m_pBigBulletTextures[Enums::BulletType::Charge] = new Texture{ textPath + "Charge/Charge_Special.png" };
	m_pBigBulletTextures[Enums::BulletType::Chaser] = new Texture{ textPath + "Chaser/Chaser_Special_Loop.png" };

	m_pChargeBulletManager = new ChargeBulletManager{};

}

BulletManager::~BulletManager()
{
	for (int i{}; i < m_pBullets.size(); i++)
	{
		delete m_pBullets[i];
	}
	m_pBullets.clear();

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		delete m_pBigBullets[i];
	}
	m_pBigBullets.clear();


	for (std::pair<Enums::BulletType, Texture*> pair : m_pSmallBulletTextures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSmallBulletTextures.clear();
	delete m_pSmallChargeTexture;
	m_pSmallChargeTexture = nullptr;

	for (std::pair<Enums::BulletType, Texture*> pair : m_pBigBulletTextures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pBigBulletTextures.clear();

	if (m_pChargeBulletManager != nullptr)
	{
		delete m_pChargeBulletManager;
		m_pChargeBulletManager = nullptr;
	}
}

void BulletManager::Draw()
{
	for (int i{}; i < m_pBullets.size(); i++)
	{
		if (m_pBullets[i]->IsActivated())m_pBullets[i]->Draw();
		
	}

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		if (m_pBigBullets[i]->IsActivated())m_pBigBullets[i]->Draw();
	}

	if (m_CurrentBulletType == Enums::BulletType::Charge && m_Shooting)
	{
		m_pChargeBulletManager->Draw();
	}
}

void BulletManager::Update(ObjectManager* objectManager, float elapsedSec, const Level* level, bool shooting, const Point2f& pos, float angle, const Rectf& player)
{
	DeleteFinishedBullets();

	if (!m_Shooting && shooting)
	{
		m_Shooting = true;
		if (m_CurrentBulletType == Enums::BulletType::PeaShooter)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::PeaShooter_Loop, 20);
		else if (m_CurrentBulletType == Enums::BulletType::SpreadShot)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::SpreadShot_Loop, 20);
		else if (m_CurrentBulletType == Enums::BulletType::Chaser)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chaser_Loop, 20);

	}
	else if (m_Shooting && !shooting)
	{
		m_Shooting = false;
		switch (m_CurrentBulletType)
		{
		case Enums::BulletType::PeaShooter:
			g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::PeaShooter_Loop);
			break;
		case Enums::BulletType::SpreadShot:
			g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::SpreadShot_Loop);
			break;
		case Enums::BulletType::Chaser:
			g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Chaser_Loop);
			break;
		default:
			break;
		}
	}

	if (m_Timer < m_FireRate)
	{
		m_Timer += elapsedSec;
	}

	if (m_CurrentBulletType == Enums::BulletType::Charge)
	{
		m_pChargeBulletManager->Update(elapsedSec, m_Shooting, pos);
		if (m_pChargeBulletManager->ShouldShootSmall())Shoot(pos, angle, false);
		else if (m_pChargeBulletManager->ShouldShootBig())Shoot(pos, angle, true);
	}
	else if (m_Timer >= m_FireRate && m_Shooting)
	{
		m_Timer -= m_FireRate;
		if (m_CurrentBulletType == Enums::BulletType::Chaser)ShootChaser(pos, angle, objectManager, level);
		else Shoot(pos, angle);

		m_ShotBullet = true;
	}

	for (int i{}; i < m_pBullets.size(); i++)
	{
		m_pBullets[i]->Update(elapsedSec, level);
	}

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		m_pBigBullets[i]->Update(elapsedSec, level, player);
	}

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		if (m_pBigBullets[i]->GetBulletType() == Enums::BulletType::Chaser)
		{
			BigChaser::UpdateAngle(elapsedSec);
			break;
		}
	}


	DeleteFinishedBullets();
}

void BulletManager::Shoot(const Point2f& pos, float angle, bool bigCharge)
{

	switch (m_CurrentBulletType)
	{
	case Enums::BulletType::PeaShooter:
		m_pBullets.push_back(new PeaShooter{m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier});
		g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::PeaShooter_Spawn);
		break;
	case Enums::BulletType::SpreadShot:
		m_pBullets.push_back(new SpreadShot{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle + float(M_PI/5.f), m_DamageMultiplier, false });
		m_pBullets.push_back(new SpreadShot{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle + float(M_PI/10.f), m_DamageMultiplier, true });
		m_pBullets.push_back(new SpreadShot{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier, false });
		m_pBullets.push_back(new SpreadShot{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle - float(M_PI/10.f), m_DamageMultiplier, true });
		m_pBullets.push_back(new SpreadShot{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle - float(M_PI/5.f), m_DamageMultiplier, false });
		g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::SpreadShot_Spawn);
		break;
	case Enums::BulletType::Roundabout:
		m_pBullets.push_back(new Roundabout{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier });
		g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Roundabout_Spawn);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Roundabout_Spawn);
		break;
	case Enums::BulletType::Lobber:
		m_pBullets.push_back(new Lobber{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier });
		g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Lobber_Spawn);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Lobber_Spawn);
		break;
	case Enums::BulletType::Charge:
		if (bigCharge)
		{
			m_pBullets.push_back(new Charge{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier, true });
			g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Charge_Big_Spawn, angle);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Charge_Big_Spawn);
		}
		else
		{
			m_pBullets.push_back(new Charge{m_pSmallChargeTexture, pos, angle, m_DamageMultiplier, false });
			g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Charge_Small_Spawn);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Charge_Small_Spawn);
		}
		break;
	default:
		m_pBullets.push_back(new PeaShooter{ m_pSmallBulletTextures[Enums::BulletType::PeaShooter], pos, angle, m_DamageMultiplier });
		g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::PeaShooter_Spawn);
		break;
	}
}

void BulletManager::ShootChaser(const Point2f& pos, float angle, ObjectManager* objectManager, const Level* level)
{
	m_pBullets.push_back(new Chaser{ m_pSmallBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier, objectManager->GetClosestTarget(pos, level) });
	g_pSFXManager->CreateParticle(pos, ParticleSoundeffectManager::ParticleType::Chaser_Spawn);
}

void BulletManager::SpecialShoot(const Point2f& pos, float angle)
{

	switch (m_CurrentBulletType)
	{
	case Enums::BulletType::PeaShooter:
		m_pBigBullets.push_back(new BigPeaShooter{ m_pBigBulletTextures[m_CurrentBulletType] , pos, angle, m_DamageMultiplier });
		break;
	case Enums::BulletType::SpreadShot:
		for (float i{}; i < 2 * M_PI; i += float(M_PI / 4))
		{
			m_pBigBullets.push_back(new BigSpreadShot{ m_pBigBulletTextures[m_CurrentBulletType] , pos, i, m_DamageMultiplier });
		}
		break;
	case Enums::BulletType::Roundabout:
		m_pBigBullets.push_back(new BigRoundabout{ m_pBigBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier });
		break;
	case Enums::BulletType::Lobber:
		  m_pBigBullets.push_back(new BigLobber{ m_pBigBulletTextures[m_CurrentBulletType] , pos, angle, m_DamageMultiplier });
		break;
	case Enums::BulletType::Charge:
		m_pBigBullets.push_back(new BigCharge{ m_pBigBulletTextures[m_CurrentBulletType], pos, angle, m_DamageMultiplier });
		break;
	case Enums::BulletType::Chaser:
		for (int i{}; i < m_pBigBullets.size(); i++)
		{
			if (m_pBigBullets[i]->GetBulletType() == Enums::BulletType::Chaser)
			{
				static_cast<BigChaser*>(m_pBigBullets[i])->Disconnect();
			}
		}

		for (int i{}; i < 4; i++)
		{
			m_pBigBullets.push_back(new BigChaser{ m_pBigBulletTextures[m_CurrentBulletType], pos, angle + (i - 2)*float(M_PI)/12.f, m_DamageMultiplier, i});
		}
		break;
	default:		
		m_pBigBullets.push_back(new BigPeaShooter{ m_pBigBulletTextures[Enums::BulletType::PeaShooter] , pos, angle, m_DamageMultiplier});
		break;
	}

}


void BulletManager::SetBullet(Enums::BulletType bulletType, const Point2f& pos, float angle)
{
	
	if (m_CurrentBulletType == Enums::BulletType::PeaShooter)g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::PeaShooter_Loop);
	else if (m_CurrentBulletType == Enums::BulletType::SpreadShot)g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::SpreadShot_Loop);
	else if (m_CurrentBulletType == Enums::BulletType::Chaser)g_pSFXManager->StopSFX(ParticleSoundeffectManager::SFXType::Chaser_Loop);
	else if (m_CurrentBulletType == Enums::BulletType::Charge)
	{
		m_pChargeBulletManager->StopShoot();
		if (m_pChargeBulletManager->ShouldShootSmall())Shoot(pos, angle, false);
		else if (m_pChargeBulletManager->ShouldShootBig())Shoot(pos, angle, true);
	}

	m_CurrentBulletType = bulletType;

	m_Shooting = false;

	m_FireRate = Bullet::GetFireRate(m_CurrentBulletType);
}


std::vector<Bullet*>& BulletManager::GetBullets()
{
	return m_pBullets;
}

std::vector<BigBullet*>& BulletManager::GetBigBullets()
{
	return m_pBigBullets;
}

Enums::BulletType BulletManager::GetBulletType()
{
	return m_CurrentBulletType;
}

bool BulletManager::ShootBullet()
{
	if (m_ShotBullet)
	{
		m_ShotBullet = false;
		return true;
	}
	return false;
}

void BulletManager::DeleteFinishedBullets()
{
	int bulletAmount{ int(m_pBullets.size()) };
	for (int i{}; i < bulletAmount; i++)
	{
		if (!m_pBullets[i]->IsActivated())
		{
			delete m_pBullets[i];
			m_pBullets[i] = nullptr;
			if (--bulletAmount > 0)
			{
				m_pBullets[i] = m_pBullets[m_pBullets.size() - 1];
				i--;
			}
			m_pBullets.pop_back();
		}
	}

	int bigBulletAmount{ int(m_pBigBullets.size()) };
	for (int i{}; i < bigBulletAmount; i++)
	{
		if (!m_pBigBullets[i]->IsActivated())
		{
			delete m_pBigBullets[i];
			m_pBigBullets[i] = nullptr;
			if (--bigBulletAmount > 0)
			{
				m_pBigBullets[i] = m_pBigBullets[m_pBigBullets.size() - 1];
				i--;
			}
			m_pBigBullets.pop_back();
		}
	}
}