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

class BulletManager final
{
public:
	

	BulletManager(float damageMultiplier, Enums::BulletType bullet1Type);
	~BulletManager();

	BulletManager(BulletManager& rhs) = delete;
	BulletManager(BulletManager&& rhs) = delete;

	BulletManager& operator=(BulletManager& rhs) = delete;
	BulletManager& operator=(BulletManager&& rhs) = delete;

	void Draw();
	void Update(ObjectManager* objectManager, float elapsedSec, const Level* level, bool shooting, const Point2f& pos, float angle, const Rectf& player);

	void SpecialShoot(const Point2f& pos, float angle);

	void SetBullet(Enums::BulletType bulletType, const Point2f& pos, float angle);

	std::vector<Bullet*>& GetBullets();
	std::vector<BigBullet*>& GetBigBullets();

	Enums::BulletType GetBulletType();

	bool ShootBullet();

private:
	std::map<Enums::BulletType, Texture*> m_pSmallBulletTextures;
	Texture* m_pSmallChargeTexture;
	std::map<Enums::BulletType, Texture*> m_pBigBulletTextures;

	std::vector<Bullet*> m_pBullets;
	std::vector<BigBullet*> m_pBigBullets;

	ChargeBulletManager* m_pChargeBulletManager{nullptr};

	bool m_Shooting{ false };

	Enums::BulletType m_CurrentBulletType;

	float m_Timer{};
	float m_FireRate{};

	float m_DamageMultiplier{};

	bool m_ShotBullet{ false };

	void Shoot(const Point2f& pos, float angle, bool bigCharge = 0);
	void ShootChaser(const Point2f& pos, float angle, ObjectManager* objectManager, const Level* level);

	void DeleteFinishedBullets();
};

