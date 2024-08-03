#include "pch.h"
#include "Hud.h"
#include "Texture.h"
#include "utils.h"
#include <string>
#include <iostream>

Hud::Hud(Enums::Character cupHead, bool canSwapWeapon, bool normalHud)
	:m_NormalHud{normalHud}
{
	std::string textPath{ "./resources/images/player/hud/" };

	if(normalHud)m_pBulletIconsTexture = new Texture{ textPath + "Bullet_Icons.png" };
	else m_pBulletIconsTexture = new Texture{ textPath + "Plane_Bullet_Icons.png" };

	m_pSwapWeaponTexture = new Texture{ textPath + "Switch_Weapon.png" };
	m_pHealthTexture = new Texture{ textPath + "Health.png" };

	if(cupHead == Enums::Character::CupHead)m_pCardTexture = new Texture{ textPath + "Cards_Red.png" };
	else m_pCardTexture = new Texture{ textPath + "Cards_Blue.png" };

	if (canSwapWeapon)m_SwapWeaponTimer = 3;
}

Hud::~Hud()
{
	delete m_pHealthTexture;
	m_pHealthTexture = nullptr;
	delete m_pCardTexture;
	m_pCardTexture = nullptr;
	delete m_pBulletIconsTexture;
	m_pBulletIconsTexture = nullptr;
	delete m_pSwapWeaponTexture;
	m_pSwapWeaponTexture = nullptr;
}

void Hud::Draw() const
{
	Rectf srcRect{};


	if (m_DrawBulletIcon)
	{
		if (m_NormalHud)
		{
			utils::CalculateSrcRect(m_pBulletIconsTexture, srcRect, 3, 3, int(m_BulletType));
		}
		else
		{
			utils::CalculateSrcRect(m_pBulletIconsTexture, srcRect, 2, 2, int(m_PlaneBulleTType));
		}
		srcRect.bottom -= srcRect.height - (srcRect.height * m_BulletIconPercentage);
		srcRect.height *= m_BulletIconPercentage;
		m_pBulletIconsTexture->Draw(Point2f{ m_Border, m_Border + m_pHealthTexture->GetHeight() / 2 }, srcRect);
	}

	utils::CalculateSrcRect(m_pHealthTexture, srcRect, m_HealtCols, m_HealthRows, m_CrHealthFrame);
	m_pHealthTexture->Draw(Point2f{ m_Border, m_Border }, srcRect);

	if (m_SwapWeaponTimer > 0)m_pSwapWeaponTexture->Draw(Point2f{ m_Border, 2 * m_Border + srcRect.height });

	DrawCards();
}

void Hud::Update(float elapsedSec, int health, float super)
{
#pragma region swapWeapon
	if (m_SwapWeaponTimer > 0)m_SwapWeaponTimer -= elapsedSec;
#pragma endregion swapWeapon

#pragma region bulletIcon
	if (m_DrawBulletIcon)
	{
		if (m_BulletIconTimer <= 0)
		{
			m_BulletIconPercentage += (3*elapsedSec * (int(m_BulletIconGoingUp) * 2 - 1));
			if (m_BulletIconGoingUp && m_BulletIconPercentage >= .7f)
			{
				m_BulletIconPercentage = .7f;
				m_BulletIconTimer = 1.f;
				m_BulletIconGoingUp = false;
			}
			else if (!m_BulletIconGoingUp && m_BulletIconPercentage <= 0.f)
			{
				m_DrawBulletIcon = false;
				m_BulletIconGoingUp = true;
			}
		}
		else m_BulletIconTimer -= elapsedSec;
	}

#pragma endregion bulletIcon

#pragma region cards
	int cardsComplete{ int(m_Super) };
	if (super < m_Super)m_Super = super;
	else if (super > m_Super)
	{
		m_Super += elapsedSec * 5;
	}

	if (super < m_Super)m_Super = super;
	if (m_Super > 5) m_Super = 5;

	if (cardsComplete < int(m_Super))m_Flipping = true;
	else if (cardsComplete == 5 && m_Super < 5)
	{
		m_Flipping = false;
		m_FlipTimer = 0;
		m_CrCardFrame = 0;
	}

	if (m_Flipping)
	{
		m_FlipTimer += elapsedSec;
		if(m_FlipTimer >= m_FrameSpeed)
		{ 
			m_FlipTimer -= m_FrameSpeed;
			if (++m_CrCardFrame >= m_NrCardFrames)
			{
				m_CrCardFrame = 0;
				if (m_Super < 5)m_Flipping = false;
			}
		}
	}
	else m_FlipTimer = 0;
#pragma endregion cards

#pragma region health
	m_CrHealthFrame = health;

	if (health > 1)
	{
		m_CrHealthFrame = health + 2;
	}
	else if (health == 1)
	{
		m_HealthTimer += elapsedSec;
		if (m_HealthTimer >= m_FrameSpeed*3)
		{
			m_HealthTimer -= m_FrameSpeed*3;
		}

		m_CrHealthFrame = int(m_HealthTimer / m_FrameSpeed) + 1;
	}
	else
	{
		m_CrHealthFrame = 0;
	}
#pragma endregion health
}

void Hud::SwapWeapon(Enums::BulletType bulletType)
{
	m_DrawBulletIcon = true;
	m_BulletIconGoingUp = true;
	m_BulletType = bulletType;
	m_BulletIconPercentage = 0.01f;
	m_BulletIconTimer = 0;
}

void Hud::SwapWeapon(Enums::PlaneBulletType bulletType)
{
	m_DrawBulletIcon = true;
	m_BulletIconGoingUp = true;
	m_PlaneBulleTType = bulletType;
	m_BulletIconPercentage = 0.01f;
	m_BulletIconTimer = 0;
}

void Hud::DrawCards() const
{
	int cardsCompleted{ int(m_Super) };

	float super{ m_Super };
	Point2f pos{ m_Border + m_pHealthTexture->GetWidth()/6 + m_Border, m_Border };

	Rectf srcRect{};
	srcRect.height = m_pCardTexture->GetHeight();
	srcRect.width = m_pCardTexture->GetWidth() / float(m_NrCardFrames);
	srcRect.bottom = srcRect.height;
	srcRect.left = 0;

	if (cardsCompleted < 5)
	{
		srcRect.left = srcRect.width * 5;
		for (int i{1}; i < cardsCompleted; i++)
		{
			m_pCardTexture->Draw(pos, srcRect);
			pos.x += srcRect.width;
		}

		if (cardsCompleted >= 1)
		{
			if (m_Flipping)srcRect.left = srcRect.width * m_CrCardFrame;
			m_pCardTexture->Draw(pos, srcRect);
			pos.x += srcRect.width;
		}

		if (m_Super - cardsCompleted > 0)
		{
			srcRect.left = 0;
			srcRect.height *= m_Super - cardsCompleted;
			srcRect.bottom *= m_Super - cardsCompleted;
			m_pCardTexture->Draw(pos, srcRect);
		}
	}
	else
	{
		srcRect.left = srcRect.width * m_CrCardFrame;
		for (int i{}; i < cardsCompleted; i++)
		{
			m_pCardTexture->Draw(pos, srcRect);
			pos.x += srcRect.width;
		}
	}



	/*while (super > 0)
	{
		if (super >= 1)
		{
			Rectf srsRect{ 0, m_pCardTexture->GetHeight(), m_pCardTexture->GetWidth()/2, m_pCardTexture->GetHeight() };
			m_pCardTexture->Draw(pos, srsRect);
			super--;
			pos.x += srsRect.width;
		}
		else
		{
			Rectf srsRect{ m_pCardTexture->GetWidth()/2, m_pCardTexture->GetHeight() * super,
				m_pCardTexture->GetWidth() / 2, m_pCardTexture->GetHeight()*super};
			m_pCardTexture->Draw(pos, srsRect);
			super = 0;
		}
	}*/

}