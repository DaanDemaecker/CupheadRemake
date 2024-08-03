#pragma once
class Level;
class SpriteManager;
class ParticleSoundeffectManager;
class Hud;
class Bullet;
class BigBullet;
class ObjectManager;
class Shadow;

#include "Avatar.h"
#include "BulletManager.h"
#include "Enums.h"
#include "Vector2f.h"
#include <vector>
#include <map>

class NormalAvatar final :  public Avatar
{
public:
	enum class State
	{
		intro = 0,
		idle = 1,
		aiming = 2,
		shooting = 3,
		run = 4,
		run_shoot = 5,
		run_shoot_dia = 6,
		duck = 7,
		duck_shoot = 8,
		jump = 9,
		parry = 10,
		parried = 11,
		parry_axe = 12,
		dash = 13,
		dash_air = 14,
		special_atk_gr = 15,
		special_atk_air = 16,
		hit_gr = 17,
		hit_air = 18,
		death = 19
	};

	explicit NormalAvatar(Controller* controller, const PlayerEntry& player, const Level* level);
	~NormalAvatar();

	virtual void SaveToFile() override;

	virtual void Update(float elapsedSec, const Level* level, ObjectManager* objectManager) override;
	virtual void Draw() const override;
	virtual void DrawHud() const override;

	virtual void SetShooting(bool shooting) override;
	virtual void SpecialAtk() override;

	virtual void Hit() override;
	virtual void Parry() override;
	virtual void IncreaseSuper(float amount) override;

	virtual bool IsDead() const override;
	virtual Rectf& GetShape() override;
	virtual bool GetParry() const override;
	virtual Vector2f& GetVelocity() override;
	virtual bool CanGetHit() const override;
	virtual float GetParryDamage() const override;
	virtual std::vector<Bullet*>& GetBullets() override;
	virtual std::vector<BigBullet*>& GetBigBullets() override;

private:
	SpriteManager* m_pSpriteManager;
	BulletManager* m_pBulletManager;
	Shadow* m_pShadow;
	Hud* m_pHud;

	const float m_AxeDamage{ 16.f };

	PlayerEntry m_PlayerEntry;
	bool m_FirstBulletType{ true };

	bool m_Jumping{ false };
	float m_JumpTimer{};
	const float m_JumpTime{ .2f };

	float m_ParryTimer{};
	const float m_ParryTime{ .2f };
	int m_SequentialParries{};

	float m_HitTimer{};
	const float m_HitTime{ 1.f };

	Rectf m_Shape;
	float m_HalfWidth;
	float m_HalfHeight;

	float m_Super{};
	int m_Health;
	int m_AimAngle{ 2 };
	bool m_FacingRight{ true };
	int m_SpecialAttackAimAngle{ 2 };
	bool m_TimedFacingRight{ true };

	const float m_HorSpeed;
	const float m_JumpSpeed;

	Vector2f m_Velocity;
	Vector2f m_Acceleration;

	State m_State;
	State m_PreviousState;

	bool m_PassingTroughPlatform{ false };
	float m_PassingTroughTimer{};
	const float m_PassingTroughTime{ .5f };

	bool m_Timed{ true };
	bool m_OnGround{ true };
	bool m_Parrying{ false };
	bool m_Shooting{ false };
	bool m_ShouldShoot{ false };
	bool m_NoMove{ false };
	bool m_CanDash{ true };
	bool m_CanSpecialAtk{ true };
	bool m_SpecialAtkShot{ false };

	bool m_WalkDustSpawned{ false };

	float m_MatchTimer{};
	int m_HitAmount{};
	int m_SpecialUsed{};
	int m_ParryAmount{};
	int m_DashAmount{};

	bool m_SmokeDashing{};

	void MovementInput(const Level* level, const ObjectManager* objectManager);
	void UpdatePosition(float elapsedSec, const Level* level, const ObjectManager* objectManager);

	void CalculateAngle();
	void CalculateState(const Level* level, ObjectManager* objectManager);

	float CalculateBulletAngle(bool specialAttack = false);
	Point2f CalculateBulletPos(bool specialAttack = false);

	void IsStateTimed();
	void HandleTimer();

	void Jump(ObjectManager* objectManager);
	void StopJump();
	bool Dash();
	bool ShouldSmokeDash();
	void SwapBullets();
	void RandomizeBullets();

	void Dashing();
	void SpecialATKing();
	void Parrying(float elapsedSec, const Level* level);

	void WalkDust();
	bool ShouldDraw() const;
};

