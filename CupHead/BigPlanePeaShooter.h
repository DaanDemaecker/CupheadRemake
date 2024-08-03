#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;


class BigPlanePeaShooter final : public BigBullet
{
public:
	explicit BigPlanePeaShooter(Texture* texturePtr, const Point2f& center, float damageMultiplier);
	virtual ~BigPlanePeaShooter();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;

private:
	BulletSpriteSheet* m_pTexture;

	float m_Speed{ 1500 };

	const float m_Damage{ 15.f };

	float m_HitTimer{};
	const float m_HitDelay{ .1f };
	bool m_CanHit{ true };



	void CheckBoundaries();
	void BulletHit();
};



