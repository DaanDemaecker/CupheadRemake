#include "pch.h"
#include "SpriteManager.h"
#include <iostream>
#include <string>

#include "SpriteSheet.h"
#include "JumpSpriteSheet.h"
#include "OneCycleSpriteSheet.h"
#include "RunSpriteSheet.h"
#include "Idle_AimSpriteSheet.h"
#include "ShootSpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "SpecialAttackSpriteSheet.h"

SpriteManager::SpriteManager(Enums::Character CupHead)
{
	std::string textPath{ "./resources/Images/Player/" };
	if (CupHead == Enums::Character::CupHead)textPath += "CupHead/";
	else if (CupHead == Enums::Character::MugMan) textPath += "MugMan/";


	m_pSprites[NormalAvatar::State::intro] = new OneCycleSpriteSheet(textPath + "Intro.png", 7, 4, 28);
	m_pSprites[NormalAvatar::State::idle] = new Idle_AimSpriteSheet(textPath + "Idle.png", 5, 1, 5);
	m_pSprites[NormalAvatar::State::aiming] = new Idle_AimSpriteSheet(textPath + "Aiming.png", 5, 5, 25);
	m_pSprites[NormalAvatar::State::shooting]  = new ShootSpriteSheet(textPath + "Shooting.png", 6, 5, 15, 3);
	m_pSprites[NormalAvatar::State::run] = new RunSpriteSheet(textPath + "Run.png", 4, 4, 16, 1/22.f);
	m_pSprites[NormalAvatar::State::run_shoot]= new RunSpriteSheet(textPath + "Run_Shoot_Straight.png", 4, 4, 16);
	m_pSprites[NormalAvatar::State::run_shoot_dia] = new RunSpriteSheet(textPath + "Run_Shoot_Diagonal.png", 4, 4, 16);
	m_pSprites[NormalAvatar::State::duck] = new LoopSpriteSheet(textPath + "Duck.png", 4, 4, 15, 5, 14);
	m_pSprites[NormalAvatar::State::duck_shoot] = new LoopSpriteSheet(textPath + "Duck_Shoot.png", 6, 1, 6, 0, 2);
	m_pSprites[NormalAvatar::State::jump] = new JumpSpriteSheet(textPath + "Jump.png", 4, 2, 8);
	m_pSprites[NormalAvatar::State::parry] = new JumpSpriteSheet(textPath + "Parry.png", 4, 2, 8);
	m_pSprites[NormalAvatar::State::parried]= new JumpSpriteSheet(textPath + "Parried.png", 4, 2, 8);
	m_pSprites[NormalAvatar::State::parry_axe] = new JumpSpriteSheet(textPath + "Parry_Axe.png", 4, 2, 8);
	m_pSprites[NormalAvatar::State::dash]= new OneCycleSpriteSheet(textPath + "Dash_Ground.png", 4, 2, 8, 1/20.f);
	m_pSprites[NormalAvatar::State::dash_air] = new OneCycleSpriteSheet(textPath + "Dash_Air.png", 4, 2, 8);
	m_pSprites[NormalAvatar::State::special_atk_gr] = new SpecialAttackSpriteSheet(textPath + "Special_Attack_Ground.png", 7, 5, 35);
	m_pSprites[NormalAvatar::State::special_atk_air] = new SpecialAttackSpriteSheet(textPath + "Special_Attack_Air.png", 6, 5, 30, 1/10.f);
	m_pSprites[NormalAvatar::State::hit_gr] = new OneCycleSpriteSheet(textPath + "Hit_Ground.png", 3, 2, 6);
	m_pSprites[NormalAvatar::State::hit_air] = new OneCycleSpriteSheet(textPath + "Hit_Air.png", 3, 2, 6);
	m_pSprites[NormalAvatar::State::death] = new OneCycleSpriteSheet(textPath + "Death.png", 4, 4, 16);

	m_pGhostSpriteSheet = new SpriteSheet(textPath + "Ghost.png", 6, 4, 24);
}

SpriteManager::~SpriteManager()
{
	for (std::pair <NormalAvatar::State, SpriteSheet*> pair : m_pSprites)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSprites.clear();

	delete m_pGhostSpriteSheet;
	m_pGhostSpriteSheet = nullptr;

}

void SpriteManager::Draw(NormalAvatar::State state, const Point2f& pos, bool facingRight)
{
	m_pSprites[state]->DrawBottom(pos, facingRight);

	if (state == NormalAvatar::State::death)
	{
		m_pGhostSpriteSheet->DrawBottom(m_GhostPos);
	}
}

void SpriteManager::Update(float elapsedSec, NormalAvatar::State state)
{
	m_pSprites[state]->Update(elapsedSec);

	if (state == NormalAvatar::State::death)
	{
		m_pGhostSpriteSheet->Update(elapsedSec);
		m_GhostPos.y += elapsedSec * m_GhostSpeed;
	}
}

void SpriteManager::UpdateAngle(int angle)
{
	m_Angle = angle;;

	static_cast<Idle_AimSpriteSheet*>(m_pSprites[NormalAvatar::State::aiming])->UpdateAngle(m_Angle);
	static_cast<ShootSpriteSheet*>(m_pSprites[NormalAvatar::State::shooting])->UpdateAngle(m_Angle);
	static_cast<SpecialAttackSpriteSheet*>(m_pSprites[NormalAvatar::State::special_atk_gr])->UpdateAngle(m_Angle);
	static_cast<SpecialAttackSpriteSheet*>(m_pSprites[NormalAvatar::State::special_atk_air])->UpdateAngle(m_Angle);
}

bool SpriteManager::GetFinishedCycle(NormalAvatar::State state)
{
	if (state == NormalAvatar::State::intro || state == NormalAvatar::State::dash || state == NormalAvatar::State::dash_air ||
		state == NormalAvatar::State::hit_gr || state == NormalAvatar::State::hit_air || state == NormalAvatar::State::death)
	{
		return static_cast<OneCycleSpriteSheet*>(m_pSprites[state])->GetFinishedCycle();
	}
	else if (state == NormalAvatar::State::special_atk_gr || state == NormalAvatar::State::special_atk_air)
	{
		return static_cast<SpecialAttackSpriteSheet*>(m_pSprites[state])->GetFinishedCycle();
	}
	else return true;
}

void SpriteManager::ResetCycle(NormalAvatar::State state)
{
	if (state == NormalAvatar::State::intro || state == NormalAvatar::State::dash || state == NormalAvatar::State::dash_air ||
		state == NormalAvatar::State::hit_gr || state == NormalAvatar::State::hit_air || state == NormalAvatar::State::death)
	{
		static_cast<OneCycleSpriteSheet*>(m_pSprites[state])->ResetCycle();
	}
	else if (state == NormalAvatar::State::special_atk_gr || state == NormalAvatar::State::special_atk_air)
	{
		static_cast<SpecialAttackSpriteSheet*>(m_pSprites[state])->ResetCycle();
	}
	else if (state == NormalAvatar::State::duck)static_cast<LoopSpriteSheet*>(m_pSprites[state])->ResetCycle();
}

int SpriteManager::GetCrFrame(NormalAvatar::State state)
{
	return m_pSprites[state]->GetCrFrame();
}

void SpriteManager::SetGhostPos(const Point2f& pos)
{
	m_GhostPos = pos;
}

void SpriteManager::ShootBullet(NormalAvatar::State state)
{
	if (state == NormalAvatar::State::duck_shoot)
	{
		int shootFrame{ 3 };
		static_cast<LoopSpriteSheet*>(m_pSprites[state])->StopLoop();
		m_pSprites[state]->SetFrame(shootFrame);
	}
	else if (state == NormalAvatar::State::shooting)
	{
		int shootFrame{ 3 };
		static_cast<ShootSpriteSheet*>(m_pSprites[state])->StopLoop();
		m_pSprites[state]->SetFrame(shootFrame);
	}
}
