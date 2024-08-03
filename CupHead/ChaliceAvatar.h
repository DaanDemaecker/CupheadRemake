#pragma once
class Level;
class ChaliceSpriteManager;
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

class ChaliceAvatar final : public Avatar
{
public:
	enum class State
	{
		intro,
		idle,
		aiming,
		shooting,
		run,
		run_shoot,
		run_shoot_dia,
		duck,
		duck_shoot,
		jump,
		jump_shoot,
		double_jump,
		dash,
		dash_air,
		rolling,
		special_atk_gr,
		special_atk_air,
		hit_gr,
		hit_air,
		death
	};

	explicit ChaliceAvatar(Controller* controller, const PlayerEntry& player, const Level* level);
	~ChaliceAvatar();

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
	ChaliceSpriteManager* m_pSpriteManager;
	BulletManager* m_pBulletManager;
	Shadow* m_pShadow;
	Hud* m_pHud;

	PlayerEntry m_PlayerEntry;
	bool m_FirstBulletType{ true };

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
	bool m_Shooting{ false };
	bool m_ShouldShoot{ false };
	bool m_NoMove{ false };
	bool m_CanDash{ true };
	bool m_CanSpecialAtk{ true };
	bool m_SpecialAtkShot{ false };

	bool m_DoubleJumped{ false };
	bool m_CanDoubleJump{ true };

	bool m_WalkDustSpawned{ false };

	float m_MatchTimer{};
	int m_HitAmount{};
	int m_SpecialUsed{};
	int m_ParryAmount{};

	void Input(const Level* level, const ObjectManager* objectManager);
	void UpdatePosition(float elapsedSec, const Level* level, const ObjectManager* objectManager);

	void CalculateAngle();
	void CalculateState(const Level* level, ObjectManager* objectManager);

	float CalculateBulletAngle(bool specialAttack = false);
	Point2f CalculateBulletPos(bool specialAttack = false);

	void IsStateTimed();
	void HandleTimer();

	void Jump(ObjectManager* objectManager);
	bool Dash();
	void SwapBullets();

	void Dashing();
	void SpecialATKing();

	void WalkDust();
	bool ShouldDraw() const;
};

