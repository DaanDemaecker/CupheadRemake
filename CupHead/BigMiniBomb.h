#pragma once
#include "BigBullet.h"
#include "SpriteSheet.h"
#include <memory>
class Texture;
class BulletSpriteSheet;
class Level;

class BigMiniBomb final : public BigBullet
{
public:
	explicit BigMiniBomb(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, std::weak_ptr<Point2f> target);
	virtual ~BigMiniBomb() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;
private:
	BulletSpriteSheet* m_pTexture;

	void CheckBoundaries();

	float m_Speed;
	float m_Damage;

	const float m_RotSpeed{ float(M_PI) / 2 };
	std::weak_ptr<Point2f> m_pTarget;
	bool m_TargetExpired{ false };
};

