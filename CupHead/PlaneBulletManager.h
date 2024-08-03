#pragma once
#include "Vector2f.h"
#include "Enums.h"
#include <vector>
#include <map>
class Bullet;
class BigBullet;
class Level;
class ParticleSoundeffectManager;
class Texture;
class ChargeBulletManager;
class ObjectManager;

class PlaneBulletManager final
{
public:


	PlaneBulletManager(Enums::Character character, float damageMultiplier, Enums::PlaneBulletType bullet1Type);
	~PlaneBulletManager();

	PlaneBulletManager(PlaneBulletManager& rhs) = delete;
	PlaneBulletManager(PlaneBulletManager&& rhs) = delete;

	PlaneBulletManager& operator=(PlaneBulletManager& rhs) = delete;
	PlaneBulletManager& operator=(PlaneBulletManager&& rhs) = delete;

	void Draw();
	void Update(float elapsedSec, ObjectManager* objectManager, const Level* level, bool shooting, const Point2f& pos, const Rectf& player);

	void SpecialShoot(const Point2f& pos, ObjectManager* objectManager, const Level* level);

	void SetBullet(Enums::PlaneBulletType bulletType);

	std::vector<Bullet*>& GetBullets();
	std::vector<BigBullet*>& GetBigBullets();

private:
	std::map<Enums::PlaneBulletType, Texture*> m_pSmallBulletTextures;
	std::map<Enums::PlaneBulletType, Texture*> m_pBigBulletTextures;

	std::vector<Bullet*> m_pBullets;
	std::vector<BigBullet*> m_pBigBullets;

	bool m_Shooting{ false };

	Enums::PlaneBulletType m_CurrentBulletType;

	float m_Timer{};
	float m_FireRate{};

	float m_DamageMultiplier;

	float m_SecondBigBombTimer{};
	const float m_SecondBigBombTime{ .1f };

	void Shoot(const Point2f& pos);

	void DeleteFinishedBullets();

	void ShootBigMiniBomb(bool first, const Point2f& pos, const Level* level, ObjectManager* objectManager);
};

