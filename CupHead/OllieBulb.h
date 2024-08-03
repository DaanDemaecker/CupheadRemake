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

class OllieBulb final
{
public:
	OllieBulb();
	~OllieBulb();

	OllieBulb(OllieBulb& rhs) = delete;
	OllieBulb(OllieBulb&& rhs) = delete;

	OllieBulb& operator=(OllieBulb& rhs) = delete;
	OllieBulb& operator=(OllieBulb&& rhs) = delete;

	void Draw();
	void Update(float elapsedSec, const Level* level, std::vector<std::weak_ptr<Point2f>>& targets);

	bool GetActivated();
	bool ActivateRadish();

	bool CheckParry(const Rectf& shape, float damage);
	bool CeckHits(Rectf& shape, bool parry);

	float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets);

	float GetHealth();
	float GetMaxHealth();

	

private:
enum class State
	{
		Intro = 0,
		Idle = 1,
		Attacking = 2,
		Dying = 3,
		Happy = 4
	};

	State m_State{ State::Intro };

	Rectf m_HitBox;
	std::shared_ptr<Point2f> m_pTarget;

	const float m_MaxHealth{ 400 };

	float m_Health;

	LoopSpriteSheet* m_pGroundBreakingTexture;
	LoopSpriteSheet* m_pCryingLoopTexture;

	std::map<State, SpriteSheet*> m_pSpriteSheets;
	std::vector<Object*> m_pObjects;
	
	bool m_Hit{ false };
	float m_HitTimer{};
	const float m_HitTime{ 0.05f };
	
	bool m_Crying{ false };

	bool m_Activated{ true };

	bool m_GroundBroken{ false };

	float m_HappyTimer{};
	const float m_HappyTime{ 5.f };

	float m_Percentage{ 1.f };
	bool m_IsBackInGround{ false };

	float m_DeathTimer{};
	const float m_DeathTime{ 2 };

	float m_DeathEffectTimer{};
	const float m_DeathEffectTime{ 0.5f };

	float m_Timer{};
	float m_AttackTimer{};

	float m_ObjectTimer{};
	float m_ObjectDelay{2.f};
	int m_CurrentObject{};

	float m_ObjectSpeed{ 650 };

	void UpdateIntro(float elapsedSec);
	void UpdateIdle(float elapsedSec);
	void UpdateAttack(float elapsedSec);
	void UpdateDeath(float elapsedSec);
	void UpdateHappy(float elapsedSec);

	void NewObject();
};

