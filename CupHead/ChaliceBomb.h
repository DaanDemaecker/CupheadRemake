#pragma once
#include "Bullet.h"
class BulletSpriteSheet;
class Level;
#include "Vector2f.h"

class ChaliceBomb final : public Bullet
{
public:
	explicit ChaliceBomb(Texture* texturePtr, const Point2f& center, float damageMultiplier);
	virtual ~ChaliceBomb() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level) override;

	virtual void Hit() override;

	virtual float GetDamage() const override;
private:

	BulletSpriteSheet* m_pTexture;

	void CheckBoundaries(const Level* level);

	float m_Damage;
	Vector2f m_Speed;
	const float m_Gravity{ -2700 };

};

