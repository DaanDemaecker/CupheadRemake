#include "pch.h"
#include "PlaneBulletManager.h"
#include "Texture.h"
#include "ObjectManager.h"
#include "Level.h"

#include "Bullet.h"
#include "PlanePeaShooter.h"
#include "MiniBomb.h"
#include "ChalicePeaShooter.h"
#include "ChaliceBomb.h"
#include "MiniPeaShooter.h"

#include "BigBullet.h"
#include "BigPlanePeaShooter.h"
#include "BigMiniBomb.h"

#include "BigChaliceBomb.h"

PlaneBulletManager::PlaneBulletManager(Enums::Character character, float damageMultiplier, Enums::PlaneBulletType bullet1Type)
	:m_DamageMultiplier{damageMultiplier}
{
	m_CurrentBulletType = bullet1Type;

	m_FireRate = Bullet::GetFireRate(bullet1Type);

	std::string textPath{ "./resources/Images/Player/Plane Bullets/" };

	std::string characterString{};

	if (character == Enums::Character::CupHead)characterString = "CupHead_";
	else characterString = "MugMan_";

	m_pSmallBulletTextures[Enums::PlaneBulletType::PeaShooter] = new Texture{ textPath + "Peashooter/" + characterString +"Peashooter_Loop.png" };
	m_pSmallBulletTextures[Enums::PlaneBulletType::Mini_Bomb] = new Texture{ textPath + "Mini_Bomb/" + characterString + "Mini_Bomb_Loop.png" };
	m_pSmallBulletTextures[Enums::PlaneBulletType::Chalice_PeaShooter] = new Texture{ textPath + "Chalice_PeaShooter/Chalice_PeaShooter_Loop.png" };
	m_pSmallBulletTextures[Enums::PlaneBulletType::Chalice_Bomb] = new Texture{ textPath + "Chalice_Bomb/Chalice_Bomb_Loop.png" };
	m_pSmallBulletTextures[Enums::PlaneBulletType::Mini] = new Texture{ textPath + "Mini_PeaShooter/Mini_PeaShooter_Loop.png" };

	
	m_pBigBulletTextures[Enums::PlaneBulletType::PeaShooter] = new Texture{ textPath + "Peashooter/" + characterString + "BigPeaShooter_Loop.png" };
	m_pBigBulletTextures[Enums::PlaneBulletType::Mini_Bomb] = new Texture{ textPath + "Mini_Bomb/" + characterString + "Big_Mini_Bomb_Loop.png" };

	m_pBigBulletTextures[Enums::PlaneBulletType::Chalice_Bomb] = new Texture{ textPath + "Chalice_Bomb/Big_Chalice_Bomb_Loop.png" };
}

PlaneBulletManager::~PlaneBulletManager()
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


	for (std::pair<Enums::PlaneBulletType, Texture*> pair : m_pSmallBulletTextures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSmallBulletTextures.clear();

	for (std::pair<Enums::PlaneBulletType, Texture*> pair : m_pBigBulletTextures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pBigBulletTextures.clear();
}

void PlaneBulletManager::Draw()
{
	for (int i{}; i < m_pBullets.size(); i++)
	{
		if (m_pBullets[i]->IsActivated())m_pBullets[i]->Draw();

	}

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		if (m_pBigBullets[i]->IsActivated())m_pBigBullets[i]->Draw();
	}
}

void PlaneBulletManager::Update(float elapsedSec, ObjectManager* objectManager, const Level* level, bool shooting, const Point2f& pos, const Rectf& player)
{
	DeleteFinishedBullets();

	if (!m_Shooting && shooting)
	{
		m_Shooting = true;
	}
	else if (m_Shooting && !shooting)
	{
		m_Shooting = false;
	}

	if (m_Timer < m_FireRate)
	{
		m_Timer += elapsedSec;
	}

	if (m_SecondBigBombTimer > 0)
	{
		m_SecondBigBombTimer -= elapsedSec;
		if (m_SecondBigBombTimer <= 0)ShootBigMiniBomb(false, pos, level, objectManager);
	}


	if (m_Timer >= m_FireRate && m_Shooting)
	{
		m_Timer -= m_FireRate;
		Shoot(pos);
	}

	for (int i{}; i < m_pBullets.size(); i++)
	{
		m_pBullets[i]->Update(elapsedSec, level);
	}

	for (int i{}; i < m_pBigBullets.size(); i++)
	{
		m_pBigBullets[i]->Update(elapsedSec, level, player);
	}


	DeleteFinishedBullets();
}

void PlaneBulletManager::SpecialShoot(const Point2f& pos, ObjectManager* objectManager, const Level* level)
{
	switch (m_CurrentBulletType)
	{
	case Enums::PlaneBulletType::PeaShooter:
		m_pBigBullets.push_back(new BigPlanePeaShooter{ m_pBigBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	case Enums::PlaneBulletType::Mini_Bomb:
		ShootBigMiniBomb(true, pos, level, objectManager);
		m_SecondBigBombTimer = m_SecondBigBombTime;
		break;

	case Enums::PlaneBulletType::Chalice_Bomb:
		m_pBigBullets.push_back(new BigChaliceBomb{ m_pBigBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	default:
		m_pBigBullets.push_back(new BigPlanePeaShooter{ m_pBigBulletTextures[Enums::PlaneBulletType::PeaShooter], pos, m_DamageMultiplier });
		break;
	}
}

void PlaneBulletManager::SetBullet(Enums::PlaneBulletType bulletType)
{
	m_CurrentBulletType = bulletType;
	m_FireRate = Bullet::GetFireRate(bulletType);
}

std::vector<Bullet*>& PlaneBulletManager::GetBullets()
{
	return m_pBullets;
}

std::vector<BigBullet*>& PlaneBulletManager::GetBigBullets()
{
	return m_pBigBullets;
}

void PlaneBulletManager::Shoot(const Point2f& pos)
{
	switch (m_CurrentBulletType)
	{
	case Enums::PlaneBulletType::PeaShooter:
		m_pBullets.push_back(new PlanePeaShooter{ m_pSmallBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	case Enums::PlaneBulletType::Mini_Bomb:
		m_pBullets.push_back(new MiniBomb{ m_pSmallBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	case Enums::PlaneBulletType::Chalice_PeaShooter:
		for (double angle{ M_PI / 18 }; angle >= -M_PI / 18; angle -= M_PI / 18)
		{
			m_pBullets.push_back(new ChalicePeaShooter{ m_pSmallBulletTextures[m_CurrentBulletType], pos, float(angle), m_DamageMultiplier });
		}
		break;
	case Enums::PlaneBulletType::Chalice_Bomb:
		m_pBullets.push_back(new ChaliceBomb{ m_pSmallBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	case Enums::PlaneBulletType::Mini:
		m_pBullets.push_back(new MiniPeaShooter{ m_pSmallBulletTextures[m_CurrentBulletType], pos, m_DamageMultiplier });
		break;
	default:
		m_pBullets.push_back(new PlanePeaShooter{ m_pSmallBulletTextures[Enums::PlaneBulletType::PeaShooter], pos, m_DamageMultiplier });
	}
}

void PlaneBulletManager::DeleteFinishedBullets()
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

void PlaneBulletManager::ShootBigMiniBomb(bool first, const Point2f& pos, const Level* level, ObjectManager* objectManager)
{
	if (first)
	{
		for (double angle{ M_PI / 5 }; angle >= -M_PI / 5; angle -= M_PI / 10)
		{
			m_pBigBullets.push_back(new BigMiniBomb{ m_pBigBulletTextures[m_CurrentBulletType], pos, float(angle), m_DamageMultiplier, objectManager->GetClosestTarget(pos, level) });
		}
	}
	else
	{
		for (double angle{ M_PI / 6 }; angle >= -M_PI / 5; angle -= M_PI / 9)
		{
			m_pBigBullets.push_back(new BigMiniBomb{ m_pBigBulletTextures[m_CurrentBulletType], pos, float(angle), m_DamageMultiplier, objectManager->GetClosestTarget(pos, level) });
		}
	}
}

