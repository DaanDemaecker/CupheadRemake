#pragma once
#include <string>
#include "BulletManager.h"
class Texture;
class Level;
class BulletSpriteSheet;

class Bullet
{
public:
	explicit Bullet(const Point2f& center, float angle, float damageMultiplier, float size);
	virtual ~Bullet();

	Bullet(Bullet& rhs) = delete;
	Bullet(Bullet&& rhs) = delete;

	Bullet& operator=(Bullet& rhs) = delete;
	Bullet& operator=(Bullet&& rhs) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const Level* level) = 0;

	virtual void Hit() = 0;

	bool IsActivated();
	virtual float GetDamage() const = 0;

	Circlef GetHitbox();

	static float GetFireRate(Enums::BulletType bulletType);
	static float GetFireRate(Enums::PlaneBulletType bulletType);

protected:
	Point2f m_StartLocation;
	Circlef m_HitBox;

	float m_Angle;
	
	float m_DamageMultiplier;

	const float m_WindowWidth{ 1280 };
	const float m_WindowHeight{720};
	bool m_IsActivated;
};



