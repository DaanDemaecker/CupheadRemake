#pragma once
#include "Bullet.h"
class Texture;
class BulletSpriteSheet;
class Level;

class ChalicePeaShooter final : public Bullet
{
public:
	explicit ChalicePeaShooter(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~ChalicePeaShooter() override;

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

