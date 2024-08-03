#include "pch.h"
#include "PlaneAvatar.h"
#include "Level.h"
#include "utils.h"
#include "ParticleSoundeffectManager.h"
#include "PlaneBulletManager.h"
#include "ObjectManager.h"
#include "Hud.h"
#include "Shadow.h"
#include "Controls.h"
#include "SpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "ReturningLoopSpriteSheet.h"
#include <iostream>
#include <string>
#include <fstream>

PlaneAvatar::PlaneAvatar(Controller* controller, Enums::Character character, Enums::Charm charm)
	:Avatar{controller},
	m_Character{character}, m_Charm{charm}
{
	m_Shape = Rectf{ 200.f, 350.f, m_Width, m_Height };

	float damageMultiplier{ 1.f };

	if (m_Charm == Enums::Charm::Heart)
	{
		m_Health = 4;
		damageMultiplier = .95f;
	}
	else if (m_Charm == Enums::Charm::TwinHearts)
	{
		m_Health = 5;
		damageMultiplier = .90f;
	}

	std::string characterString{};
	if (charm == Enums::Charm::AstralCookie)
	{
		m_Health = 4;
		m_BulletType1 = Enums::PlaneBulletType::Chalice_PeaShooter;
		m_BulletType2 = Enums::PlaneBulletType::Chalice_Bomb;
		if (m_Character == Enums::Character::CupHead)characterString = "CupHead_";
		else characterString = "MugMan_";
	}
	else
	{
		m_BulletType1 = Enums::PlaneBulletType::PeaShooter;
		m_BulletType2 = Enums::PlaneBulletType::Mini_Bomb;
	}

	m_CurrentInput[Enums::Actions::Up] = false;
	m_CurrentInput[Enums::Actions::Down] = false;
	m_CurrentInput[Enums::Actions::Left] = false;
	m_CurrentInput[Enums::Actions::Right] = false;
	m_CurrentInput[Enums::Actions::Jump] = false;
	m_CurrentInput[Enums::Actions::Dash] = false;
	m_CurrentInput[Enums::Actions::Aim] = false;
	m_CurrentInput[Enums::Actions::Shoot] = false;
	m_CurrentInput[Enums::Actions::Ex] = false;
	m_CurrentInput[Enums::Actions::SwapBullets] = false;

	m_PreviousInput = m_CurrentInput;

	m_pHud = new Hud{ character, true, false };

	m_pBulletManager = new PlaneBulletManager{ character, damageMultiplier, m_BulletType1 };

	std::string textPath{ "./resources/images/player/" };
	if (charm == Enums::Charm::AstralCookie)
	{
		textPath += "Chalice Plane/";
	}
	else
	{
		if (character == Enums::Character::CupHead) textPath += "CupHead Plane/";
		else textPath += "MugMan Plane/";
	}

	if (charm == Enums::Charm::AstralCookie) m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + characterString + "Intro.png", 11, 4, 41 };
	else m_pSpriteSheets[State::Intro] = new OneCycleSpriteSheet{ textPath + "Intro.png", 11, 4, 41 };
	
	m_pSpriteSheets[State::Parry] = new OneCycleSpriteSheet{ textPath + "Parry.png", 5, 4, 20, 1/20.f};
	m_pSpriteSheets[State::Parry_Axe] = new OneCycleSpriteSheet{ textPath + "Parry_Axe.png", 5, 4, 20};
	m_pSpriteSheets[State::Special_Up] = new OneCycleSpriteSheet{ textPath + "Special_Up.png", 7, 3, 21};
	m_pSpriteSheets[State::Special_Down] = new OneCycleSpriteSheet{ textPath + "Special_Down.png", 7, 3, 21};
	m_pSpriteSheets[State::Idle] = new SpriteSheet{ textPath + "Idle.png", 4, 1, 4 };
	m_pSpriteSheets[State::Up] = new ReturningLoopSpriteSheet{ textPath + "Up.png", 5, 3, 15, 10 };
	m_pSpriteSheets[State::Down] = new ReturningLoopSpriteSheet{ textPath + "Down.png", 5, 3, 15, 10 };
	m_pSpriteSheets[State::Morph_Shrink] = new OneCycleSpriteSheet{ textPath + "Morph_Shrink.png", 2, 2, 4, 1/35.f };
	m_pSpriteSheets[State::Morph_Grow] = new OneCycleSpriteSheet{ textPath + "Morph_Grow.png", 2, 2, 4, 1/35.f };
	m_pSpriteSheets[State::Shrunk_Idle] = new SpriteSheet{ textPath + "Shrunk_Idle.png", 4, 1, 4 };
	m_pSpriteSheets[State::Shrunk_Up] = new ReturningLoopSpriteSheet{ textPath + "Shrunk_Up.png", 5, 3, 15, 10};
	m_pSpriteSheets[State::Shrunk_Down] = new ReturningLoopSpriteSheet{ textPath + "Shrunk_Down.png", 5, 3, 15, 10 };
	m_pSpriteSheets[State::Death] = new SpriteSheet{ textPath + "Ghost.png", 6, 4, 24 };
}

PlaneAvatar::~PlaneAvatar()
{
	delete m_pHud;
	m_pHud = nullptr;

	delete m_pBulletManager;
	m_pBulletManager = nullptr;

	for (std::pair <PlaneAvatar::State, SpriteSheet*> pair : m_pSpriteSheets)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSpriteSheets.clear();
}

void PlaneAvatar::SaveToFile()
{
	std::ofstream file;
	file.open("./resources/TextFiles/MatchResult.txt");
	std::string line;

	if (m_ParryAmount > 3)m_ParryAmount = 3;

	if (file.is_open())
	{
		line = "<Time=\"" + std::to_string(int(m_MatchTimer)) + "\">\n";
		file << line;
		line = "<HpBonus=\"" + std::to_string(3 - m_HitAmount) + "\">\n";
		file << line;
		line = "<ParryAmount=\"" + std::to_string(m_ParryAmount) + "\">\n";
		file << line;
		line = "<SuperMeter=\"" + std::to_string(m_SpecialUsed) + "\">\n";
		file << line;
	}

	file.close();
}

void PlaneAvatar::Update(float elapsedSec, const Level* level, ObjectManager* objectManager)
{
	m_MatchTimer += elapsedSec;

	Avatar::CharmSuperCharge(elapsedSec, m_Super, m_Charm);

	if (m_HitTimer > 0)
	{
		m_HitTimer -= elapsedSec;
	}

	if (!m_ShouldShoot && IsShooting() && !m_Shrunk && Avatar::RelicEquiped(m_Charm))SwapBullets();
	m_ShouldShoot = IsShooting();

	m_pHud->Update(elapsedSec, m_Health, m_Super);
	m_pBulletManager->Update(elapsedSec, objectManager, level, m_ShouldShoot, Point2f{ m_Shape.left + m_Shape.width*1.3f, m_Shape.bottom + m_Shape.height / 2 }, m_Shape);

	m_pSpriteSheets[m_State]->Update(elapsedSec);
	if (m_State == State::Death)m_Shape.bottom += m_GhostSpeed * elapsedSec;
	else
	{
		HandleTimers(level, objectManager);
		CalculateState();

		UpdatePosition(elapsedSec, level);
	}
}

void PlaneAvatar::Draw() const
{
	if (ShouldDraw())m_pSpriteSheets.at(m_State)->DrawCenterRotated(Point2f{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 }, CalculateTilt());
	m_pBulletManager->Draw();

	utils::SetColor( Color4f{1.f, 0.f, 0.f, 1.f} );
	//utils::FillRect(m_Shape);
}

void PlaneAvatar::DrawHud() const
{
	m_pHud->Draw();
}

void PlaneAvatar::SetShooting(bool shooting)
{
	m_Shooting = shooting;
}

void PlaneAvatar::Parry()
{
	m_Parrying = false;
	m_ParryAmount++;
	IncreaseSuper(1.f);

	if (m_Charm == Enums::Charm::HeartRing || m_Charm == Enums::Charm::DivineRelic)
	{
		if (m_ParryAmount == 1 || m_ParryAmount == 3 || m_ParryAmount == 6)m_Health++;
	}
	else if (m_Charm == Enums::Charm::CursedRelic1)
	{
		if (m_ParryAmount == 3 || m_ParryAmount == 6 || m_ParryAmount == 10)m_Health++;
	}
	else if (m_Charm == Enums::Charm::CursedRelic2)
	{
		if (m_ParryAmount == 2 || m_ParryAmount == 5 || m_ParryAmount == 9)m_Health++;
	}
	else if (m_Charm == Enums::Charm::CursedRelic3)
	{
		if (m_ParryAmount == 1 || m_ParryAmount == 4 || m_ParryAmount == 8)m_Health++;
	}
	else if (m_Charm == Enums::Charm::CursedRelic4)
	{
		if (m_ParryAmount == 1 || m_ParryAmount == 3 || m_ParryAmount == 7)m_Health++;
	}
}

void PlaneAvatar::SpecialAtk()
{
	if ((m_State == State::Idle || m_State == State::Up) && m_Super >= 1)
	{
		m_State = State::Special_Up;
		IncreaseSuper(-1);
		if (++m_SpecialUsed >= 6)m_SpecialUsed = 6;
	}
	else if (m_State == State::Down && m_Super >= 1)
	{
		m_State = State::Special_Down;
		IncreaseSuper(-1);
		if (++m_SpecialUsed >= 6)m_SpecialUsed = 6;
	}
}

void PlaneAvatar::Hit()
{
	if (++m_HitAmount >= 3)m_HitAmount = 3;
	m_HitTimer = m_HitTime;
	m_Health--;
	
	if (m_Health == 0)
	{
		m_State = State::Death;
	}
}

void PlaneAvatar::IncreaseSuper(float amount)
{
	m_Super += amount;
	if (m_Super > 5)m_Super = 5;
}

bool PlaneAvatar::IsDead() const
{
	return (m_State == State::Death);
}

Rectf& PlaneAvatar::GetShape()
{
	return m_Shape;
}

bool PlaneAvatar::GetParry() const
{
	return m_Parrying;
}

Vector2f& PlaneAvatar::GetVelocity()
{
	return m_Velocity;
}

bool PlaneAvatar::CanGetHit() const
{
	if (m_HitTimer <= 0 && m_State != State::Special_Down && m_State != State::Special_Up 
		&& m_State !=State::Death && m_State != State::Intro)return true;
	else return false;
}

float PlaneAvatar::GetParryDamage() const
{
	if (m_Charm == Enums::Charm::Whetstone)return m_AxeDamage;
	else return 0;
}

std::vector<Bullet*>& PlaneAvatar::GetBullets()
{
	return m_pBulletManager->GetBullets();
}

std::vector<BigBullet*>& PlaneAvatar::GetBigBullets()
{
	return m_pBulletManager->GetBigBullets();
}

bool PlaneAvatar::ShouldDraw() const
{
	if (m_HitTimer > 0)
	{
		if (int(m_HitTimer * 9) % 3 == 1)return false;
	}
	return true;
}

float PlaneAvatar::CalculateTilt() const
{
	if (m_State != State::Morph_Grow && m_State != State::Morph_Shrink && m_State != State::Intro && m_State != State::Parry)
	{
		float tilt{ -15 };
		float percentage{};

		if (m_Shrunk)percentage = m_Velocity.x / (m_MaxSpeed * m_ShrinkMultiplier);
		else percentage = m_Velocity.x / m_MaxSpeed;

		return tilt * percentage;
	}
	else return 0.f;
}

void PlaneAvatar::CalculateState()
{
	Avatar::Input(true);

	for (auto& pair : m_PreviousInput)
	{
		switch (pair.first)
		{
		case Enums::Actions::Ex:
			if (!pair.second && m_CurrentInput[pair.first])
			{
				SpecialAtk();
			}
			break;
		case Enums::Actions::Aim:
			if (!pair.second && m_CurrentInput[pair.first] 
				&& m_State != State::Special_Down && m_State != State::Special_Up)
			{
				m_State = State::Morph_Shrink;
			}
			else if (pair.second && !m_CurrentInput[pair.first])
			{
				m_State = State::Morph_Grow;
			}
			break;
		case Enums::Actions::Dash:
			if (!pair.second && m_CurrentInput[pair.first]
				&& m_State != State::Special_Down && m_State != State::Special_Up)
			{
				m_State = State::Morph_Shrink;
			}
			else if (pair.second && !m_CurrentInput[pair.first])
			{
				m_State = State::Morph_Grow;
			}
			break;
		case Enums::Actions::SwapBullets:
			if (!pair.second && m_CurrentInput[pair.first])
				SwapBullets();
			break;
		case Enums::Actions::Jump:
			if (!pair.second && m_CurrentInput[pair.first] && !m_Shrunk
				&& m_State != State::Intro && m_State != State::Morph_Grow && m_State != State::Morph_Shrink
				&& m_State != State::Special_Down && m_State != State::Special_Up)
			{
				if (Avatar::ShouldAxeParry(m_Charm, m_ParryAmount))
				{
					if (m_State != State::Parry_Axe)
					{
						m_State = State::Parry_Axe;
						m_Parrying = true;
					}
				}
				else
				{
					if (m_State != State::Parry)
					{
						m_State = State::Parry;
						m_Parrying = true;
					}
				}
			}
		case Enums::Actions::Up:
			if (pair.second && !m_CurrentInput[pair.first])
			{
				if (m_Shrunk)static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[State::Shrunk_Up])->ReturnCycle();
				else static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[State::Up])->ReturnCycle();
			}
			break;
		case Enums::Actions::Down:
			if (pair.second && !m_CurrentInput[pair.first])
			{
				if (m_Shrunk)static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[State::Shrunk_Down])->ReturnCycle();
				else static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[State::Down])->ReturnCycle();
			}
			break;
		default:
			break;
		}

		pair.second = m_CurrentInput[pair.first];
	}

	if (m_State != State::Intro)
	{
		m_Up = m_CurrentInput[Enums::Actions::Up];
		m_Down = m_CurrentInput[Enums::Actions::Down];
		m_Left = m_CurrentInput[Enums::Actions::Left];
		m_Right = m_CurrentInput[Enums::Actions::Right];

		if (m_State != State::Parry && m_State != State::Parry_Axe && m_State != State::Morph_Grow
			&& m_State != State::Morph_Shrink && m_State != State::Special_Down && m_State != State::Special_Up)
		{
			if (m_Up)
			{
				if (m_Shrunk && m_State == State::Shrunk_Idle)m_State = State::Shrunk_Up;
				else if(m_State == State::Idle) m_State = State::Up;
			}
			else if (m_Down)
			{
				if (m_Shrunk && m_State == State::Shrunk_Idle)m_State = State::Shrunk_Down;
				else if(m_State == State::Idle) m_State = State::Down;
			}
		}
	}

	if (m_State != m_PreviousState)
	{
		if (m_PreviousState == State::Up || m_PreviousState == State::Down || m_PreviousState == State::Shrunk_Up || m_PreviousState == State::Shrunk_Down)
			static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[m_PreviousState])->ResetCycle();
		else if (m_PreviousState == State::Parry || m_PreviousState == State::Parry_Axe)
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_PreviousState])->ResetCycle();
		}
		m_PreviousState = m_State;
	}

}

void PlaneAvatar::HandleTimers(const Level* level, ObjectManager* objectManager)
{
	int stopParryFrame{ 11 }, shootSpecialFrame{13};

	switch (m_State)
	{
	case PlaneAvatar::State::Intro:
		if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Idle;
		}
		break;
	case PlaneAvatar::State::Parry:
	case PlaneAvatar::State::Parry_Axe:
		if (m_pSpriteSheets[m_State]->GetCrFrame() == stopParryFrame)
		{
			if(!m_Parrying)
			{
				static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
				m_State = State::Idle;
			}
		}
		else if (m_pSpriteSheets[m_State]->GetCrFrame() == stopParryFrame + 1)
		{
			if (m_Parrying)m_Parrying = false;
		}
		else if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Idle;
		}
		break;
	case PlaneAvatar::State::Special_Up:
	case PlaneAvatar::State::Special_Down:
		if (m_pSpriteSheets[m_State]->GetCrFrame() == shootSpecialFrame && !m_SpecialAtkShot)
		{
			m_pBulletManager->SpecialShoot(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height / 2 }, objectManager, level);
			m_SpecialAtkShot = true;
		}
		else if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Idle;
			m_SpecialAtkShot = false;
		}
		break;
	case PlaneAvatar::State::Morph_Shrink:
		if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Shrunk_Idle;
			m_Shrunk = true;
			GrowAndShrink();
		}
		break;
	case PlaneAvatar::State::Morph_Grow:
		if (static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->GetFinishedCycle())
		{
			static_cast<OneCycleSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Idle;
			m_Shrunk = false;
			GrowAndShrink();
		}
		break;
	case PlaneAvatar::State::Down:
	case PlaneAvatar::State::Up:
		if (static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[m_State])->HasReturned())
		{
			static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Idle;
		}
		break;
	case PlaneAvatar::State::Shrunk_Down:
	case PlaneAvatar::State::Shrunk_Up:
		if (static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[m_State])->HasReturned())
		{
			static_cast<ReturningLoopSpriteSheet*>(m_pSpriteSheets[m_State])->ResetCycle();
			m_State = State::Shrunk_Idle;
		}
		break;
	}
}

bool PlaneAvatar::IsShooting()
{
	if (m_State == State::Intro || m_State == State::Special_Down || m_State == State::Special_Up)return false;
	else return (m_Shooting || m_CurrentInput[Enums::Actions::Shoot]);
}

void PlaneAvatar::UpdatePosition(float elapsedSec, const Level* level)
{
	Vector2f tempVelocity{};

	float multiplier{ 1.f };
	if (m_Shrunk)multiplier = m_ShrinkMultiplier;

	if (m_Up)m_Velocity.y += m_Acceleration * multiplier * elapsedSec;
	else if (m_Down) m_Velocity.y -= m_Acceleration * multiplier * elapsedSec;
	else MoveCloserToZero(m_Velocity.y, m_Acceleration * multiplier * elapsedSec);

	if (m_Right)m_Velocity.x += m_Acceleration * multiplier * elapsedSec;
	else if (m_Left) m_Velocity.x -= m_Acceleration * multiplier * elapsedSec;
	else MoveCloserToZero(m_Velocity.x, m_Acceleration * multiplier * elapsedSec);

	if (m_Velocity.y > m_MaxSpeed * multiplier) m_Velocity.y = m_MaxSpeed * multiplier;
	else if (m_Velocity.y < -m_MaxSpeed * multiplier) m_Velocity.y = -m_MaxSpeed * multiplier;
	
	if (m_Velocity.x > m_MaxSpeed * multiplier) m_Velocity.x = m_MaxSpeed * multiplier;
	else if (m_Velocity.x < -m_MaxSpeed * multiplier) m_Velocity.x = -m_MaxSpeed * multiplier;

	tempVelocity = m_Velocity;
	if (m_HitTimer > 0)tempVelocity.x -= m_HitKnockbackSpeed * (m_HitTimer / m_HitTime);

	level->HandleCollision(m_Shape, tempVelocity);

	m_Shape.left += tempVelocity.x * elapsedSec;
	m_Shape.bottom += tempVelocity.y * elapsedSec;
}

void PlaneAvatar::MoveCloserToZero(float& number, float step)
{
	if (number > 0)
	{
		number -= step;
		if (number < 0) number = 0;
	}
	else if (number < 0)
	{
		number += step;
		if (number > 0) number = 0;
	}
}

void PlaneAvatar::SwapBullets()
{
	m_FirstBulletType = !m_FirstBulletType;

	if (m_FirstBulletType)
	{
		m_pBulletManager->SetBullet(m_BulletType1);
		m_pHud->SwapWeapon(m_BulletType1);
	}
	else
	{
		m_pBulletManager->SetBullet(m_BulletType2);
		m_pHud->SwapWeapon(m_BulletType2);
	}
}

void PlaneAvatar::GrowAndShrink()
{
	Point2f center{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };

	if (m_Shrunk)
	{
		m_Shape = Rectf{ center.x - m_SmallWidth / 2, center.y - m_SmallHeight / 2, m_SmallWidth, m_SmallHeight };

		m_pBulletManager->SetBullet(Enums::PlaneBulletType::Mini);
	}
	else
	{
		m_Shape = Rectf{ center.x - m_Width / 2, center.y - m_Height / 2, m_Width, m_Height };

		if (m_FirstBulletType)m_pBulletManager->SetBullet(m_BulletType1);
		else m_pBulletManager->SetBullet(m_BulletType2);
	}
}
