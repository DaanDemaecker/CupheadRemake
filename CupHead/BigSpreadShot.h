#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;


class BigSpreadShot final : public BigBullet
{
public:
	explicit BigSpreadShot(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~BigSpreadShot();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;

	virtual Enums::BulletType GetBulletType() const override;

private:
	BulletSpriteSheet* m_pTexture;

	float m_TrailTimer{};
	const float m_TrailTime{ .08f };

	const float m_Speed{ 750 };

	const float m_Damage{ 5.f };

	void CheckBoundaries();
	void BulletHit();
};



