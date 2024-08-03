#include "pch.h"
#include "ChargeBulletManager.h"
#include "SpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

ChargeBulletManager::ChargeBulletManager()
{
	std::string textPath{ "./resources/Images/Player/Bullets/Charge/" };

	m_pSmallCharging1Texture = new SpriteSheet{textPath + "Charge_Small_Charging1.png", 3, 2, 6};
	m_pSmallCharging2Texture = new OneCycleSpriteSheet{textPath + "Charge_Small_Charging2.png", 3, 2, 5};
	m_pBigCharging1Texture = new SpriteSheet{textPath + "Charge_Big_Charging1.png", 3 ,2, 6};
	m_pBigCharging2Texture = new SpriteSheet{textPath + "Charge_Big_Charging2.png", 3, 3, 5};
}

ChargeBulletManager::~ChargeBulletManager()
{
	delete m_pSmallCharging1Texture;
	m_pSmallCharging1Texture = nullptr;

	delete m_pSmallCharging2Texture;
	m_pSmallCharging2Texture = nullptr;

	delete m_pBigCharging1Texture;
	m_pBigCharging1Texture = nullptr;

	delete m_pBigCharging2Texture;
	m_pBigCharging2Texture = nullptr;
}

void ChargeBulletManager::Draw() const
{
	if (!m_Charged)
	{
		m_pSmallCharging1Texture->DrawCenter(m_Pos);
		m_pSmallCharging2Texture->DrawCenter(m_Pos);
	}
	else
	{
		m_pBigCharging1Texture->DrawCenter(m_Pos);
		m_pBigCharging2Texture->DrawCenter(m_Pos);
	}
}

void ChargeBulletManager::Update(float elapsedSec, bool shooting, const Point2f& pos)
{
	m_Pos = pos;

	if (!m_Shooting && shooting)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Charge_Charging);
	if (m_Shooting && !shooting)StopShoot();
	
	m_Shooting = shooting;

	if (m_Shooting)
	{

		if (!m_Charged)
		{
			m_pSmallCharging1Texture->Update(elapsedSec);
			m_pSmallCharging2Texture->Update(elapsedSec);

			if (static_cast<OneCycleSpriteSheet*>(m_pSmallCharging2Texture)->GetFinishedCycle())
			{
				static_cast<OneCycleSpriteSheet*>(m_pSmallCharging2Texture)->ResetCycle();
				if (++m_SmallChargingLoops >= 3)
				{
					g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Charge_Charged);
					m_Charged = true;
					m_SmallChargingLoops = 0;
				}
			}
		}
		else
		{
			m_pBigCharging1Texture->Update(elapsedSec);
			m_pBigCharging2Texture->Update(elapsedSec);
		}
	}
}

bool ChargeBulletManager::ShouldShootSmall()
{
	if (m_ShouldShootSmall)
	{
		m_SmallChargingLoops = 0;
		m_ShouldShootSmall = false;
		return true;
	}
	return false;
}

bool ChargeBulletManager::ShouldShootBig()
{
	if (m_ShouldShootBig)
	{
		m_Charged = false;
		m_ShouldShootBig = false;
		return true;
	}
	return false;
}

void ChargeBulletManager::StopShoot()
{
	if (m_Charged)m_ShouldShootBig = true;
	else if (m_Shooting) m_ShouldShootSmall = true;
}