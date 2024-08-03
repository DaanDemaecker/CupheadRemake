#pragma once
#include <vector>
#include "Vector2f.h"
#include <map>
#include <memory>

class Object;
class SpriteSheet;
class LoopSpriteSheet;
class Level;
class Bullet;
class BigBullet;

class SalSpudder final
{
public:
	SalSpudder();
	~SalSpudder();

	SalSpudder(SalSpudder& rhs) = delete;
	SalSpudder(SalSpudder&& rhs) = delete;

	SalSpudder& operator=(SalSpudder& rhs) = delete;
	SalSpudder& operator=(SalSpudder&& rhs) = delete;

	void Draw();
	void Update(float elapsedSec, const Level* level, std::vector<std::weak_ptr<Point2f>>& targets);

	bool GetActivated();

	bool CheckParry(const Rectf& shape, float damage);
	bool CeckHits(Rectf& shape, bool parry);

	float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets);

	float GetHealth();
	float GetMaxHealth();

	enum class State
	{
		Intro = 0,
		Idle = 1,
		Attacking = 2,
		Dying = 3
	};
private:
	State m_State{ State::Intro };

	Rectf m_HitBox;
	float m_MaxHealth{360};
	float m_Health;

	LoopSpriteSheet* m_pGroundBreakingTexture;
	std::map<State, SpriteSheet*> m_pSpriteSheets;
	std::vector<Object*> m_pObjects;

	std::shared_ptr<Point2f> m_pTarget;

	bool m_Activated{true};

	bool m_GroundBroken{ false };

	bool m_Hit{ false };
	float m_HitTimer{};
	const float m_HitTime{ 0.05f };

	bool m_IsBackInGround{ false };
	float m_Percentage{ 1.f };

	float m_Timer{};
	int m_Attack{};

	float m_DeathTimer{};
	const float m_DeathTime{ 2 };

	float m_DeathEffectTimer{};
	const float m_DeathEffectTime{ .5f };

	Point2f m_ObjectSpawn;
	float m_ObjectSpeed{ 650 };

	void UpdateIntro(float elapsedSec);
	void UpdateIdle(float elapsedSec);
	void UpdateAttack(float elapsedSec);
	void UpdateDeath(float elapsedSec);
};

