#pragma once
#include <string>
#include "BulletManager.h"
class Texture;
class Level;
class BulletSpriteSheet;

class BigBullet
{
public:
	explicit BigBullet(const Point2f& center, float angle, float damageMultiplier, float size);
	virtual ~BigBullet();

	BigBullet(BigBullet& rhs) = delete;
	BigBullet(BigBullet&& rhs) = delete;

	BigBullet& operator=(BigBullet& rhs) = delete;
	BigBullet& operator=(BigBullet&& rhs) = delete;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) = 0;

	virtual bool Hit() = 0;

	bool IsActivated() const;
	virtual float GetDamage() const = 0;

	Circlef GetHitbox() const;

	virtual Enums::BulletType GetBulletType() const;

protected:
	Point2f m_StartLocation;
	Circlef m_HitBox;

	float m_Angle;

	float m_DamageMultiplier;

	const float m_WindowWidth{ 1280 };
	const float m_WindowHeight{ 720 };
	bool m_IsActivated;
};