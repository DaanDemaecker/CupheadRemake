#pragma once
#include <vector>
#include <map>
#include "Vector2f.h"
#include <memory>


class Object;
class SpriteSheet;
class LoopSpriteSheet;
class ParticleSpriteSheet;
class Level;
class Bullet;
class BigBullet;
class Carrot;
class Texture;

class ChaunceyChantenay final
{
public:
	ChaunceyChantenay();
	~ChaunceyChantenay();

	ChaunceyChantenay(ChaunceyChantenay& rhs) = delete;
	ChaunceyChantenay(ChaunceyChantenay&& rhs) = delete;

	ChaunceyChantenay& operator=(ChaunceyChantenay& rhs) = delete;
	ChaunceyChantenay& operator=(ChaunceyChantenay&& rhs) = delete;

	void DrawCarrotIntro() const;
	void Draw();
	void DrawObjects() const;
	void FixCameraMove(float cameraDistance);
	void Update(float elapsedSec, const Level* level, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets);
	void UpdateDeath(float elapsedSec);

	bool IsDead();

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
		Dying = 3
	};

	State m_State{ State::Intro };

	Point2f m_ObjectSpawn;
	Point2f m_TranslatedObjectSpawn;
	Point2f m_HitboxCorner;
	Rectf m_HitBox;
	std::shared_ptr<Point2f> m_pTarget;

	float m_Health;
	const float m_MaxHealth{ 475 };

	LoopSpriteSheet* m_pGroundBreakingTexture;
	ParticleSpriteSheet* m_pBeamChargeSpriteSheet;
	std::map<State, SpriteSheet*> m_pSpriteSheets;
	std::vector<Object*> m_pObjects;
	std::vector<Carrot*> m_pCarrots;

	bool m_GroundBroken{ false };

	bool m_Hit{ false };
	float m_HitTimer{};
	const float m_HitTime{ 0.05f };

	float m_CarrotTimer{};
	int m_CarrotNumber{};

	int m_Attack{};
	float m_AttackTimer{};
	const float m_AttackTime{ 2 };

	Texture* m_pBeamChargeTexture;
	bool m_ChargingBeam{ false };
	bool m_DoingBeamAttack{false};
	bool m_BeamAttackDone{ false };
	float m_BeamAttackTimer{};
	const float m_BeamAttackDelay{ 0.1f };
	int m_BeamAttackCurrent{};
	const int m_BeamAttackAmount{ 5 };
	float m_BeamAttackAngle{};

	Point2f m_Pos;

	const float m_ObjectSpeed{ 650 };

	float m_DeathEffectTimer{};
	const float m_DeathEffectTime{ 0.5f };

	void UpdateIntro(float elapsedSec);
	void UpdateIdle(float elapsedSec);
	void UpdateAttack(float elapsedSec, const Rectf& shape);

	void UpdateBeamAttack(float elapsedSec);	
	void UpdateObjects(float elapsedSec, const Level* level, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets);
	void DeleteInactiveObjects();

};

