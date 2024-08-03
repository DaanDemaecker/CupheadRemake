#pragma once
#include "Bullet.h"
class Texture;
class BulletSpriteSheet;
class Level;

class Roundabout final : public Bullet
{
public:
	explicit Roundabout(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~Roundabout() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level) override;

	virtual void Hit() override;

	virtual float GetDamage() const override;
private:
	BulletSpriteSheet* m_pTexture;

	void CheckBoundaries();

	const float m_StartSpeed;

	float m_Speed;
	float m_Damage;

	float m_Acceleration;
};

