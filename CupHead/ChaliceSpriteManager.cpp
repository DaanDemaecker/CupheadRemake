#include "pch.h"
#include "ChaliceSpriteManager.h"
#include <iostream>
#include <string>

#include "SpriteSheet.h"
#include "JumpLoopSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "RunSpriteSheet.h"
#include "Idle_AimSpriteSheet.h"
#include "ShootSpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "SpecialAttackSpriteSheet.h"

ChaliceSpriteManager::ChaliceSpriteManager(Enums::Character CupHead)
{
	std::string textPath{ "./resources/Images/Player/Chalice/" };

	std::string characterString{};
	if (CupHead == Enums::Character::CupHead)characterString += "CupHead_";
	else if (CupHead == Enums::Character::MugMan) characterString += "MugMan_";

	m_pSprites[ChaliceAvatar::State::intro] = new OneCycleSpriteSheet{ textPath + characterString + "Intro.png", 10, 6, 60 };
	m_pSprites[ChaliceAvatar::State::idle] = new Idle_AimSpriteSheet{ textPath + "Idle.png", 10, 1, 10 };
	m_pSprites[ChaliceAvatar::State::aiming] = new Idle_AimSpriteSheet{ textPath + "Aiming.png", 10, 5, 50 };
	m_pSprites[ChaliceAvatar::State::shooting] = new ShootSpriteSheet{ textPath + "Shooting.png", 6, 5, 30, 3 };
	m_pSprites[ChaliceAvatar::State::run] = new RunSpriteSheet{ textPath + "Run.png", 4, 4, 16 };
	m_pSprites[ChaliceAvatar::State::run_shoot] = new RunSpriteSheet{ textPath + "Run_Shoot_Straight.png", 4, 4, 16 };
	m_pSprites[ChaliceAvatar::State::run_shoot_dia] = new RunSpriteSheet{ textPath + "Run_Shoot_Diagonal.png", 4, 4, 16 };
	m_pSprites[ChaliceAvatar::State::duck] = new LoopSpriteSheet{ textPath + "Duck.png", 4, 4, 13, 5, 12 };
	m_pSprites[ChaliceAvatar::State::duck_shoot] = new LoopSpriteSheet{ textPath + "Duck_Shoot.png", 6, 1, 6, 0, 2 };
	m_pSprites[ChaliceAvatar::State::jump] = new JumpLoopSpriteSheet{ textPath + "Jump_Loop.png", 5, 4, 20, 3, 11, 12, 19 };
	m_pSprites[ChaliceAvatar::State::jump_shoot] = new JumpLoopSpriteSheet{ textPath + "Jump_Shoot_Legs.png", 5, 4, 20, 3, 11, 12, 19 };
	m_pSprites[ChaliceAvatar::State::double_jump] = new SpriteSheet{textPath + "Double_Jump.png", 4, 2, 8};
	m_pSprites[ChaliceAvatar::State::dash] = new OneCycleSpriteSheet{ textPath + "Dash_Ground.png", 4, 2, 8 };
	m_pSprites[ChaliceAvatar::State::dash_air] = new OneCycleSpriteSheet{ textPath + "Dash_Air.png", 4, 2, 8 };
	m_pSprites[ChaliceAvatar::State::rolling] = new OneCycleSpriteSheet{ textPath + "Rolling.png", 4, 2, 8 };
	m_pSprites[ChaliceAvatar::State::special_atk_gr] = new SpecialAttackSpriteSheet{ textPath + "Special_Attack_Ground.png", 11, 5, 55 };
	m_pSprites[ChaliceAvatar::State::special_atk_air] = new SpecialAttackSpriteSheet{ textPath + "Special_Attack_Air.png", 11, 5, 55 };
	m_pSprites[ChaliceAvatar::State::hit_gr] = new OneCycleSpriteSheet{ textPath + "Hit_Ground.png", 4, 2, 7 };
	m_pSprites[ChaliceAvatar::State::hit_air] = new OneCycleSpriteSheet{ textPath + "Hit_Air.png", 4, 2, 7 };
	m_pSprites[ChaliceAvatar::State::death] = new OneCycleSpriteSheet{ textPath + "Death.png", 4, 4, 16 };

	m_pJumpShootHead = new ShootSpriteSheet(textPath + "Jump_Shoot_Head.png", 3, 5, 15, 1);

	m_pGhostSpriteSheet = new SpriteSheet(textPath + "Ghost.png", 6, 4, 24);
}

ChaliceSpriteManager::~ChaliceSpriteManager()
{
	for (std::pair <ChaliceAvatar::State, SpriteSheet*> pair : m_pSprites)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSprites.clear();

	delete m_pGhostSpriteSheet;
	m_pGhostSpriteSheet = nullptr;

	delete m_pJumpShootHead;
	m_pJumpShootHead = nullptr;

}

void ChaliceSpriteManager::Draw(ChaliceAvatar::State state, const Point2f& pos, bool facingRight)
{
	m_pSprites[state]->DrawBottom(pos, facingRight);

	if (state == ChaliceAvatar::State::jump_shoot)m_pJumpShootHead->DrawBottom(pos, facingRight);

	if (state == ChaliceAvatar::State::death)
	{
		m_pGhostSpriteSheet->DrawBottom(m_GhostPos);
	}
}

void ChaliceSpriteManager::Update(float elapsedSec, ChaliceAvatar::State state, float yVelocity)
{
	m_pSprites[state]->Update(elapsedSec);

	if (state == ChaliceAvatar::State::jump_shoot)
	{
		m_pJumpShootHead->Update(elapsedSec);
	}

	if (state == ChaliceAvatar::State::death)
	{
		m_pGhostSpriteSheet->Update(elapsedSec);
		m_GhostPos.y += elapsedSec * m_GhostSpeed;
	}

	if (state == ChaliceAvatar::State::jump || state == ChaliceAvatar::State::jump_shoot)
	{
		if (yVelocity < 0 && !static_cast<JumpLoopSpriteSheet*>(m_pSprites[state])->IsLooping2())
		{
			static_cast<JumpLoopSpriteSheet*>(m_pSprites[state])->GoToSecondLoop();
		}
		else if (yVelocity >= 0 && !static_cast<JumpLoopSpriteSheet*>(m_pSprites[state])->IsLooping1())
		{
			static_cast<JumpLoopSpriteSheet*>(m_pSprites[state])->ResetCycle();
		}
	}
}

void ChaliceSpriteManager::UpdateAngle(int angle)
{
	m_Angle = angle;;

	m_pJumpShootHead->UpdateAngle(m_Angle);
	static_cast<Idle_AimSpriteSheet*>(m_pSprites[ChaliceAvatar::State::aiming])->UpdateAngle(m_Angle);
	static_cast<ShootSpriteSheet*>(m_pSprites[ChaliceAvatar::State::shooting])->UpdateAngle(m_Angle);
	static_cast<SpecialAttackSpriteSheet*>(m_pSprites[ChaliceAvatar::State::special_atk_gr])->UpdateAngle(m_Angle);
	static_cast<SpecialAttackSpriteSheet*>(m_pSprites[ChaliceAvatar::State::special_atk_air])->UpdateAngle(m_Angle);
}

bool ChaliceSpriteManager::GetFinishedCycle(ChaliceAvatar::State state)
{
	if (state == ChaliceAvatar::State::intro || state == ChaliceAvatar::State::dash || state == ChaliceAvatar::State::dash_air ||
		state == ChaliceAvatar::State::hit_gr || state == ChaliceAvatar::State::hit_air || 
		state == ChaliceAvatar::State::rolling || state == ChaliceAvatar::State::death)
	{
		return static_cast<OneCycleSpriteSheet*>(m_pSprites[state])->GetFinishedCycle();
	}
	else if (state == ChaliceAvatar::State::special_atk_gr || state == ChaliceAvatar::State::special_atk_air)
	{
		return static_cast<SpecialAttackSpriteSheet*>(m_pSprites[state])->GetFinishedCycle();
	}
	else return false;
}

void ChaliceSpriteManager::ResetCycle(ChaliceAvatar::State state)
{
	if (state == ChaliceAvatar::State::intro || state == ChaliceAvatar::State::dash || state == ChaliceAvatar::State::dash_air ||
		state == ChaliceAvatar::State::hit_gr || state == ChaliceAvatar::State::hit_air || state == ChaliceAvatar::State::death ||
		state == ChaliceAvatar::State::rolling)
	{
		static_cast<OneCycleSpriteSheet*>(m_pSprites[state])->ResetCycle();
	}
	else if (state == ChaliceAvatar::State::special_atk_gr || state == ChaliceAvatar::State::special_atk_air)
	{
		static_cast<SpecialAttackSpriteSheet*>(m_pSprites[state])->ResetCycle();
	}
	else if (state == ChaliceAvatar::State::jump || state == ChaliceAvatar::State::jump_shoot)
	{
		static_cast<JumpLoopSpriteSheet*>(m_pSprites[state])->ResetCycle();
	}
	else if (state == ChaliceAvatar::State::duck)static_cast<LoopSpriteSheet*>(m_pSprites[state])->ResetCycle();
}

int ChaliceSpriteManager::GetCrFrame(ChaliceAvatar::State state)
{
	return m_pSprites[state]->GetCrFrame();
}

void ChaliceSpriteManager::SetGhostPos(const Point2f& pos)
{
	m_GhostPos = pos;
}

void ChaliceSpriteManager::ShootBullet(ChaliceAvatar::State state)
{
	if (state == ChaliceAvatar::State::duck_shoot)
	{
		int shootFrame{ 3 };
		static_cast<LoopSpriteSheet*>(m_pSprites[state])->StopLoop();
		m_pSprites[state]->SetFrame(shootFrame);
	}
	else if (state == ChaliceAvatar::State::shooting)
	{
		int shootFrame{ 3 };
		static_cast<ShootSpriteSheet*>(m_pSprites[state])->StopLoop();
		m_pSprites[state]->SetFrame(shootFrame);
	}
	else if (state == ChaliceAvatar::State::jump_shoot)
	{
		int shootFrame{ 1 };
		m_pJumpShootHead->StopLoop();
		m_pJumpShootHead->SetFrame(shootFrame);
	}
}
