#pragma once
#include "Avatar.h"
class PlaneBulletManager;


class PlaneAvatar final : public Avatar
{
public:
	enum class State
	{
		Intro,
		Parry,
		Parry_Axe,
		Special_Up,
		Special_Down,
		Idle,
		Up,
		Down,
		Morph_Shrink,
		Morph_Grow,
		Shrunk_Idle,
		Shrunk_Up,
		Shrunk_Down,
		Death
	};

	explicit PlaneAvatar(Controller* controller, Enums::Character character, Enums::Charm charm);
	~PlaneAvatar();


	virtual void SaveToFile() override;

	virtual void Update(float elapsedSec, const Level* level, ObjectManager* objectManager) override;
	virtual void Draw() const override;
	virtual void DrawHud() const override;

	virtual void SetShooting(bool shooting) override;
	virtual void Parry() override;
	virtual void SpecialAtk() override;

	virtual void Hit() override;
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
	State m_State{State::Intro};
	State m_PreviousState{State::Intro};

	bool m_Up{ false };
	bool m_Down{ false };
	bool m_Left{ false };
	bool m_Right{ false };

	std::map<State, SpriteSheet*> m_pSpriteSheets;


	Hud* m_pHud;

	PlaneBulletManager* m_pBulletManager;

	Rectf m_Shape;

	const float m_Width{80};
	const float m_Height{50};
	const float m_SmallWidth{45};
	const float m_SmallHeight{25};

	Enums::Charm m_Charm;
	Enums::Character m_Character;
	const float m_AxeDamage{ 16.f };

	Enums::PlaneBulletType m_BulletType1;
	Enums::PlaneBulletType m_BulletType2;

	bool m_FirstBulletType{true};
	bool m_Shrunk{ false };

	float m_HitTimer{};
	const float m_HitTime{ 1.f };

	float m_Super{};
	int m_Health{3};

	Vector2f m_Velocity{};
	const float m_Acceleration{600};
	const float m_MaxSpeed{300};
	const float m_ShrinkMultiplier{ 2.f };

	const float m_GhostSpeed{ 100.f };
	const float m_HitKnockbackSpeed{ 250.f };

	float m_MatchTimer{};
	int m_HitAmount{};
	int m_SpecialUsed{};
	int m_ParryAmount{};

	bool m_Parrying{ false };
	bool m_Shooting{ false };
	bool m_ShouldShoot{ false };
	bool m_SpecialAtkShot{ false };

	bool ShouldDraw() const;
	float CalculateTilt() const;

	void CalculateState();

	void HandleTimers(const Level* level, ObjectManager* objectManager);
	bool IsShooting();

	void UpdatePosition(float elapsedSec, const Level* level);

	void MoveCloserToZero(float& number, float step);

	void GrowAndShrink();

	void SwapBullets();
};

