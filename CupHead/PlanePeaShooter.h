#pragma once
#include "Bullet.h"
class Texture;
class BulletSpriteSheet;
class Level;

class PlanePeaShooter final : public Bullet
{
public:
	explicit PlanePeaShooter(Texture* texturePtr, const Point2f& center, float damageMultiplier);
	virtual ~PlanePeaShooter() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level) override;

	virtual void Hit() override;

	virtual float GetDamage() const override;
private:
	BulletSpriteSheet* m_pTexture;

	void CheckBoundaries();

	float m_Speed;
	float m_Damage;

};

