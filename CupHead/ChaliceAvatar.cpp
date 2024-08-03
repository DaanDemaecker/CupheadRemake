#include "pch.h"
#include "ChaliceAvatar.h"
#include "Level.h"
#include "utils.h"
#include "ChaliceSpriteManager.h"
#include "ParticleSoundeffectManager.h"
#include "ObjectManager.h"
#include "Hud.h"
#include "Shadow.h"
#include "Controls.h"
#include <iostream>
#include <string>
#include <fstream>

extern ParticleSoundeffectManager* g_pSFXManager;

ChaliceAvatar::ChaliceAvatar(Controller* controller, const PlayerEntry& player, const Level* level)
	:Avatar{controller},
	m_HorSpeed{ 400.f }, m_JumpSpeed{ 950.f }, m_Health{ 4 }, m_State{ State::intro }, m_PreviousState{ State::intro },
	m_Velocity{ Vector2f{0.f, 0.f} }, m_Acceleration{ Vector2f{0.f, -2700.f} }, m_PlayerEntry{ player }
{
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

	m_pSpriteManager = new ChaliceSpriteManager{ player.character };
	m_pBulletManager = new BulletManager{ 1, player.bulletType1 };
	m_Shape = Rectf{ 200.f, 80.f, 72.f, 97.f };
	m_Shape.bottom = level->GetGroundPos(m_Shape).y;

	bool canSwapWeapon{ true };
	if (player.bulletType2 == Enums::BulletType::none)canSwapWeapon = false;
	m_pHud = new Hud{ player.character, canSwapWeapon, true };

	m_pShadow = new Shadow{};

	m_HalfWidth = m_Shape.width / 2;
	m_HalfHeight = m_Shape.height / 2;
}

ChaliceAvatar::~ChaliceAvatar()
{
	delete m_pSpriteManager;
	m_pSpriteManager = nullptr;
	delete m_pBulletManager;
	m_pBulletManager = nullptr;
	delete m_pHud;
	m_pHud = nullptr;
	delete m_pShadow;
	m_pShadow = nullptr;
}

void ChaliceAvatar::SaveToFile()
{
	std::ofstream file;
	file.open("./resources/TextFiles/MatchResult.txt");
	std::string line;

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

void ChaliceAvatar::Update(float elapsedSec, const Level* level, ObjectManager* objectManager)
{
	m_MatchTimer += elapsedSec;

	m_pShadow->Update(Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom }, level->GetGroundPos(m_Shape));

	bool tempOnGround{ level->IsOnGround(m_Shape) };
	if (!tempOnGround && m_Velocity.y <= 0)tempOnGround = objectManager->IsOnPlatform(m_Shape, m_PassingTroughPlatform);

	if (!m_OnGround && tempOnGround)
	{
		m_pSpriteManager->ResetCycle(State::jump);
		g_pSFXManager->CreateParticle(Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom }, ParticleSoundeffectManager::ParticleType::Jump_Dust);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Land_Ground);
	}

	m_OnGround = tempOnGround;

	if (m_OnGround)
	{
		m_CanDash = true;
		m_CanSpecialAtk = true;
		m_CanDoubleJump = true;
		m_DoubleJumped = false;
	}

	if (m_PassingTroughPlatform)
	{
		m_PassingTroughTimer += elapsedSec;
		if (m_PassingTroughTimer >= m_PassingTroughTime)
		{
			m_PassingTroughPlatform = false;
			m_PassingTroughTimer = 0;
		}
	}

	m_pHud->Update(elapsedSec, m_Health, m_Super);
	m_pSpriteManager->Update(elapsedSec, m_State, m_Velocity.y);

	bool shooting{ false };
	if (!m_Timed)shooting = m_Shooting || m_CurrentInput[Enums::Actions::Shoot];

	m_ShouldShoot = shooting;

	CalculateAngle();
	m_pBulletManager->Update(objectManager, elapsedSec, level, m_ShouldShoot, CalculateBulletPos(), CalculateBulletAngle(), m_Shape);
	if (m_pBulletManager->ShootBullet())m_pSpriteManager->ShootBullet(m_State);

	IsStateTimed();

	if (m_State == State::run || m_State == State::run_shoot || m_State == State::run_shoot_dia)WalkDust();

	if (m_Timed)
	{
		m_Velocity = Vector2f{ 0,0 };
		HandleTimer();
		if (m_State == State::dash || m_State == State::dash_air || m_State == State::rolling )Dashing();
		else if (m_State == State::special_atk_air || m_State == State::special_atk_gr)SpecialATKing();
	}

	if(!m_Timed)
	{
		CalculateState(level, objectManager);
		Input(level, objectManager);
		if (m_State == State::duck || m_State == State::duck_shoot)m_NoMove = true;
	}

	if (m_NoMove)m_Velocity.x = 0;
	UpdatePosition(elapsedSec, level, objectManager);
	if (m_HitTimer >= 0)m_HitTimer -= elapsedSec;
}

void ChaliceAvatar::Draw() const
{
	if (!m_OnGround)m_pShadow->Draw();

	bool facingRight{};
	if (m_Timed)facingRight = m_TimedFacingRight;
	else facingRight = m_FacingRight;

	if (ShouldDraw())m_pSpriteManager->Draw(m_State, Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom }, facingRight);
	m_pBulletManager->Draw();
}

void ChaliceAvatar::DrawHud() const
{
	m_pHud->Draw();
}

void ChaliceAvatar::SetShooting(bool shooting)
{
	m_Shooting = shooting;
}

void ChaliceAvatar::Jump(ObjectManager* objectManager)
{
	if (m_State == State::duck || m_State == State::duck_shoot)
	{
		if (objectManager->IsOnPlatform(m_Shape, m_PassingTroughPlatform))
		{
			m_PassingTroughPlatform = true;
			return;
		}
	}

	if (m_OnGround)
	{
		m_Velocity.y = m_JumpSpeed;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Jump);
	}

	if (m_CanDoubleJump && (m_State == State::jump || m_State == State::jump_shoot))
	{
		m_CanDoubleJump = false;
		m_DoubleJumped = true;
		m_Velocity.y = m_JumpSpeed;
		m_State = State::double_jump;
	}
}

void ChaliceAvatar::Parry()
{
	m_Velocity.y = 3 * m_JumpSpeed / 3;
	if (m_Super < 4)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Parry_Power);
	else g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Parry_Power_Full);

	IncreaseSuper(1);

	m_pSpriteManager->ResetCycle(m_State);

	m_State = State::jump;

	m_ParryAmount++;

	m_CanDoubleJump = true;
	m_DoubleJumped = false;
	m_CanDash = true;
}

bool ChaliceAvatar::Dash()
{
	if(m_State == State::duck || m_State == State::duck_shoot)
	{
		m_Timed = true;
		m_NoMove = false;
		m_TimedFacingRight = int(m_FacingRight);
		m_State = State::rolling;

		return true;
	}
	else if (!m_Timed && m_CanDash)
	{
		m_Timed = true;
		m_CanDash = false;
		m_NoMove = false;
		m_TimedFacingRight = int(m_FacingRight);
		if (m_OnGround)m_State = State::dash;
		else m_State = State::dash_air;

		return true;
	}
	return false;
}

void ChaliceAvatar::SpecialAtk()
{
	if (!m_Timed && m_CanSpecialAtk && m_Super >= 1)
	{
		IncreaseSuper(-1);

		if (++m_SpecialUsed >= 6)m_SpecialUsed = 6;

		m_Timed = true;
		m_CanSpecialAtk = false;
		m_TimedFacingRight = int(m_FacingRight);
		m_SpecialAttackAimAngle = m_AimAngle;
		if (m_OnGround)m_State = State::special_atk_gr;
		else m_State = State::special_atk_air;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Special_Attack);
	}
}

void ChaliceAvatar::SwapBullets()
{
	if (m_PlayerEntry.bulletType2 != Enums::BulletType::none)
	{

		m_FirstBulletType = !m_FirstBulletType;

		if (m_FirstBulletType)
		{
			m_pHud->SwapWeapon(m_PlayerEntry.bulletType1);
			m_pBulletManager->SetBullet(m_PlayerEntry.bulletType1, CalculateBulletPos(), CalculateBulletAngle());
		}
		else
		{
			m_pHud->SwapWeapon(m_PlayerEntry.bulletType2);
			m_pBulletManager->SetBullet(m_PlayerEntry.bulletType2, CalculateBulletPos(), CalculateBulletAngle());
		}

	}
}

void ChaliceAvatar::Hit()
{
	m_HitTimer = m_HitTime;

	if (++m_HitAmount >= 3)m_HitAmount = 3;

	m_Health--;
	m_Timed = true;
	m_TimedFacingRight = m_FacingRight;
	m_Velocity = Vector2f{};
	if (m_OnGround)m_State = State::hit_gr;
	else m_State = State::hit_air;
	g_pSFXManager->CreateParticle(Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom + m_HalfHeight }, ParticleSoundeffectManager::ParticleType::Hit_FX);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Damage);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Hit);
}

void ChaliceAvatar::IncreaseSuper(float amount)
{
	m_Super += amount;
	if (m_Super > 5)m_Super = 5;
}

bool ChaliceAvatar::IsDead() const
{
	if (m_State == State::death) return true;
	else return false;
}

Rectf& ChaliceAvatar::GetShape()
{
	return m_Shape;
}

bool ChaliceAvatar::GetParry() const
{
	return (m_State == State::dash || m_State == State::dash_air);
}

Vector2f& ChaliceAvatar::GetVelocity()
{
	return m_Velocity;
}

bool ChaliceAvatar::CanGetHit() const
{
	if (m_State == State::hit_air || m_State == State::hit_gr || m_State == State::death
		|| m_State == State::special_atk_air || m_State == State::special_atk_gr
		|| m_State == State::rolling || m_HitTimer > 0)return false;
	else return true;
}

float ChaliceAvatar::GetParryDamage() const
{
	return 0;
}

std::vector<Bullet*>& ChaliceAvatar::GetBullets()
{
	return m_pBulletManager->GetBullets();
}

std::vector<BigBullet*>& ChaliceAvatar::GetBigBullets()
{
	return m_pBulletManager->GetBigBullets();
}

void ChaliceAvatar::Input(const Level* level, const ObjectManager* objectManager)
{
	if (m_CurrentInput[Enums::Actions::Right])
	{
		m_Velocity.x = m_HorSpeed;
		m_FacingRight = true;
	}
	else if (m_CurrentInput[Enums::Actions::Left])
	{
		m_Velocity.x = -m_HorSpeed;
		m_FacingRight = false;
	}
	else
	{
		m_Velocity.x = 0;
	}
}

void ChaliceAvatar::UpdatePosition(float elapsedSec, const Level* level, const ObjectManager* objectManager)
{
	level->HandleCollision(m_Shape, m_Velocity);
	objectManager->HandleCollisions(m_Shape, m_Velocity, m_PassingTroughPlatform);

	if (!m_OnGround && !m_Timed)
	{
		m_Velocity.y += m_Acceleration.y * elapsedSec;;
	}
	else if (m_OnGround && m_Velocity.y < 0)
	{
		m_Velocity.y = 0;
	}

	if (!m_NoMove)m_Shape.left += m_Velocity.x * elapsedSec;

	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void ChaliceAvatar::CalculateAngle()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	bool up{ false }, down{ false }, left{ false }, right{ false };

	right = m_CurrentInput[Enums::Actions::Right];
	left = m_CurrentInput[Enums::Actions::Left];
	down = m_CurrentInput[Enums::Actions::Down];
	up = m_CurrentInput[Enums::Actions::Up];

	if (up && !(left || right))m_AimAngle = 0;
	else if (up && (left || right))m_AimAngle = 1;
	else if (!up && !down)m_AimAngle = 2;
	else if (down && (left || right))m_AimAngle = 3;
	else if (down && !(left || right))m_AimAngle = 4;

	if (m_State != State::special_atk_air && m_State != State::special_atk_gr)m_pSpriteManager->UpdateAngle(m_AimAngle);
}

void ChaliceAvatar::CalculateState(const Level* level, ObjectManager* objectManager)
{
	m_NoMove = false;

	bool stopCheck{ false };

	Avatar::Input(true);

	for (auto& pair : m_PreviousInput)
	{
		switch (pair.first)
		{
		case Enums::Actions::Dash:
			if (!pair.second && m_CurrentInput[pair.first])stopCheck = Dash();
			break;
		case Enums::Actions::SwapBullets:
			if (!pair.second && m_CurrentInput[pair.first] && !Avatar::RelicEquiped(m_PlayerEntry.charm))
				SwapBullets();
			break;
		case Enums::Actions::Jump:
			if (!pair.second && m_CurrentInput[pair.first])Jump(objectManager);
			break;
		case Enums::Actions::Ex:
			if (!pair.second && m_CurrentInput[pair.first])
			{
				SpecialAtk();
				stopCheck = true;
			}
			break;
		}

		pair.second = m_CurrentInput[pair.first];
	}


	if (!stopCheck)
	{
		if (!m_OnGround)
		{
			if (m_DoubleJumped)m_State = State::double_jump;
			else
			{
				if (m_Shooting || m_CurrentInput[Enums::Actions::Shoot]) m_State = State::jump_shoot;
				else m_State = State::jump;
			}
		}
		else
		{
			if (!(m_Shooting || m_CurrentInput[Enums::Actions::Shoot]))
			{
				if (m_CurrentInput[Enums::Actions::Aim])
				{
					m_State = State::aiming;
					m_NoMove = true;
				}
				else if (m_CurrentInput[Enums::Actions::Down])
				{
					m_State = State::duck;
					m_NoMove = true;
				}
				else if (m_CurrentInput[Enums::Actions::Left] || m_CurrentInput[Enums::Actions::Right])m_State = State::run;
				else m_State = State::idle;
			}
			else if (m_Shooting || m_CurrentInput[Enums::Actions::Shoot])
			{
				if (m_CurrentInput[Enums::Actions::Aim] && (m_Shooting || m_CurrentInput[Enums::Actions::Shoot]))
				{
					m_State = State::shooting;
					m_NoMove = true;
				}
				else if (m_CurrentInput[Enums::Actions::Down])
				{
					m_State = State::duck_shoot;
					m_NoMove = true;
				}
				else if ((m_CurrentInput[Enums::Actions::Right] || m_CurrentInput[Enums::Actions::Left])
					&& (m_CurrentInput[Enums::Actions::Up]))m_State = State::run_shoot_dia;
				else if (m_CurrentInput[Enums::Actions::Right] || m_CurrentInput[Enums::Actions::Left])m_State = State::run_shoot;
				else m_State = State::shooting;
			}
		}
	}

	if (m_State != m_PreviousState)
	{
		if (m_State == State::duck || m_State == State::duck_shoot)m_Shape.height = m_HalfHeight;
		else if ((m_State != State::duck_shoot && m_State != State::rolling ) && (m_PreviousState == State::duck || m_PreviousState == State::duck_shoot || m_PreviousState == State::rolling))
		{
			m_pSpriteManager->ResetCycle(State::duck);
			m_Shape.height = 2 * m_HalfHeight;
		}
		m_PreviousState = m_State;
	}
}

float ChaliceAvatar::CalculateBulletAngle(bool specialAttack)
{
	int aimAngle{};
	if (specialAttack)aimAngle = m_SpecialAttackAimAngle;
	else aimAngle = m_AimAngle;

	float bulletAngle{};

	if (m_State != State::duck_shoot)
	{
		if (aimAngle == 0)
		{
			bulletAngle = float(M_PI / 2);
		}
		else if (aimAngle == 1)
		{
			bulletAngle = float(M_PI / 4);
		}
		else if (aimAngle == 2)
		{
			bulletAngle = 0;
		}
		else if (aimAngle == 3)
		{
			bulletAngle = float(-M_PI / 8);
		}
		else if (aimAngle == 4)
		{
			bulletAngle = float(-M_PI / 2);
		}

		if (!m_FacingRight) {
			bulletAngle = float(M_PI) - bulletAngle;
		}


	}
	else if (m_State == State::duck_shoot)
	{
		bulletAngle = int(!m_FacingRight) * float(M_PI);
	}

	return bulletAngle;
}

Point2f ChaliceAvatar::CalculateBulletPos(bool specialAttack)
{
	int aimAngle{};
	if (specialAttack)aimAngle = m_SpecialAttackAimAngle;
	else aimAngle = m_AimAngle;

	Point2f pos{};

	float bulletAngle{};

	if (m_State != State::duck_shoot)
	{
		if (aimAngle == 0)
		{
			bulletAngle = float(M_PI / 2 - M_PI / 8);
		}
		else if (aimAngle == 1)
		{
			bulletAngle = float(M_PI / 4 - M_PI / 16);
		}
		else if (aimAngle == 2)
		{
			bulletAngle = 0;
		}
		else if (aimAngle == 3)
		{
			bulletAngle = float(-M_PI / 8);
		}
		else if (aimAngle == 4)
		{
			bulletAngle = float(-M_PI / 2 + M_PI / 8);
		}

		if (!m_FacingRight) {
			bulletAngle = float(M_PI) - bulletAngle;
		}

		if (aimAngle != 4)pos = Point2f{ m_Shape.left + m_HalfWidth + float(cos(bulletAngle)) * 1.4f * m_HalfWidth,
			m_Shape.bottom + m_HalfHeight + float(sin(bulletAngle)) * 1.5f * m_HalfHeight };
		else pos = Point2f{ m_Shape.left + m_HalfWidth + float(cos(bulletAngle)) * 1.4f * m_HalfWidth,
			m_Shape.bottom + m_HalfHeight + float(sin(bulletAngle)) * 1 * m_HalfHeight };
	}
	else if (m_State == State::duck_shoot)
	{
		bulletAngle = float(-M_PI / 8);
		if (!m_FacingRight)bulletAngle = float(M_PI) - bulletAngle;


		pos = Point2f{ m_Shape.left + m_HalfWidth + float(cos(bulletAngle)) * 1.4f * m_HalfWidth,
			m_Shape.bottom + m_HalfHeight + float(sin(bulletAngle)) * 1.4f * m_HalfHeight };
	}


	return pos;
}

void ChaliceAvatar::IsStateTimed()
{
	if ((m_State == State::intro) || (m_State == State::dash) || (m_State == State::dash_air) ||
		m_State == State::rolling || (m_State == State::special_atk_gr) || (m_State == State::special_atk_air) ||
		(m_State == State::hit_gr) || (m_State == State::hit_air) || (m_State == State::death))m_Timed = true;
	else m_Timed = false;
}

void ChaliceAvatar::HandleTimer()
{
	if (m_pSpriteManager->GetFinishedCycle(m_State) && m_State != State::death)
	{
		m_Timed = false;
		m_SpecialAtkShot = false;
		m_pSpriteManager->ResetCycle(m_State);
		if ((m_State == State::hit_air || m_State == State::hit_gr) && m_Health <= 0)
		{
			m_State = State::death;
			g_pSFXManager->StopAllSFX();
			m_pSpriteManager->SetGhostPos(Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom });
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Death);
			m_Timed = true;
		}
	}
}

void ChaliceAvatar::Dashing()
{
	m_Velocity.x = (int(m_TimedFacingRight) * 2 - 1) * 1.5f * m_HorSpeed;
}

void ChaliceAvatar::SpecialATKing()
{
	float angle{};
	if (m_SpecialAttackAimAngle == 0)angle = float(M_PI / 2);
	else if (m_SpecialAttackAimAngle == 1)angle = float(M_PI / 4);
	else if (m_SpecialAttackAimAngle == 2)angle = 0;
	else if (m_SpecialAttackAimAngle == 3)angle = float(-M_PI / 4);
	else if (m_SpecialAttackAimAngle == 4)angle = float(-M_PI / 2);

	if (m_pSpriteManager->GetCrFrame(m_State) >= 7)
	{
		m_Velocity.y = -float(sin(angle) * m_HorSpeed);
		m_Velocity.x = float(cos(angle) * m_HorSpeed);
		if (!m_SpecialAtkShot)
		{
			m_SpecialAtkShot = true;
			m_pBulletManager->SpecialShoot(CalculateBulletPos(true), CalculateBulletAngle(true));
			g_pSFXManager->CreateParticle(Point2f(m_Shape.left + m_HalfWidth, m_Shape.bottom + m_HalfHeight), ParticleSoundeffectManager::ParticleType::Special_Dust, CalculateBulletAngle(true));
		}
	}
	if (m_TimedFacingRight)m_Velocity.x *= -1;
}

void ChaliceAvatar::WalkDust()
{
	if (m_pSpriteManager->GetCrFrame(m_State) == 0 || m_pSpriteManager->GetCrFrame(m_State) == 8)
	{
		if (!m_WalkDustSpawned)
		{
			g_pSFXManager->CreateParticle(Point2f{ m_Shape.left + m_HalfWidth, m_Shape.bottom + m_HalfHeight },
				ParticleSoundeffectManager::ParticleType::Walk_Dust);
			m_WalkDustSpawned = true;
		}
	}
	else if (m_WalkDustSpawned)
	{
		m_WalkDustSpawned = false;
	}
}

bool ChaliceAvatar::ShouldDraw() const
{
	if (m_HitTimer > 0)
	{
		if (int(m_HitTimer * 9) % 3 == 1)return false;
	}

	return true;
}