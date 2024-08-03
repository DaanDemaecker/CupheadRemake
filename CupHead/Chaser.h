#pragma once
#include "Bullet.h"
#include "SpriteSheet.h"
#include <memory>
class Texture;
class BulletSpriteSheet;
class Level;

class Chaser final : public Bullet
{
public:
	explicit Chaser(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, std::weak_ptr<Point2f> target);
	virtual ~Chaser() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level) override;

	virtual void Hit() override;

	virtual float GetDamage() const override;
private:
	BulletSpriteSheet* m_pTexture;

	SpriteSheet* m_pTrail;

	void CheckBoundaries();

	float m_Speed;
	float m_Damage;

	const float m_RotSpeed{ float(M_PI)*3 };
	std::weak_ptr<Point2f> m_pTarget;
	bool m_TargetExpired{false};
};

