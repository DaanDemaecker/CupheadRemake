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

class HoraceRadiche final
{
public:
	HoraceRadiche();
	~HoraceRadiche();

	HoraceRadiche(HoraceRadiche& rhs) = delete;
	HoraceRadiche(HoraceRadiche&& rhs) = delete;

	HoraceRadiche& operator=(HoraceRadiche& rhs) = delete;
	HoraceRadiche& operator=(HoraceRadiche&& rhs) = delete;

	void Draw();
	void Update(float elapsedSec, const Level* level, const Rectf& player, std::vector<std::weak_ptr<Point2f>>& targets);

	bool GetActivated();

	bool CeckHits(Rectf& shape);

	float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets);

private:
	enum class State
	{
		Intro = 0,
		Attacking = 1,
		Dying = 2,
	};

	State m_State{ State::Intro };

	Rectf m_HitBox;
	std::shared_ptr<Point2f> m_pTarget;
	float m_Speed{};
	const float m_MaxSpeed{ 750.f };
	const float m_Acceleration{1000.f};

	const float m_MaxHealth{ 230 };

	float m_Health;

	std::map<State, SpriteSheet*> m_pSpriteSheets;

	bool m_Hit{ false };
	float m_HitTimer{};
	const float m_HitTime{ 0.05f };

	float m_DeathTimer{};
	const float m_DeathTime{ 2 };

	bool m_Activated{ true };

	void UpdateIntro(float elapsedSec);
	void UpdateAttack(float elapsedSec, const Rectf& player);
	void UpdateDeath(float elapsedSec);
};