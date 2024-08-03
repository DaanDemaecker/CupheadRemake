#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;


class BigPeaShooter final : public BigBullet
{
public:
	explicit BigPeaShooter(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~BigPeaShooter();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;
	
	virtual Enums::BulletType GetBulletType() const override;

private:
	BulletSpriteSheet* m_pTexture;

	float m_Speed{ 1500 };

	const float m_Damage{ 8.33f };

	float m_HitTimer{};
	const float m_HitDelay{ .075f };
	int m_Hits{};
	const int m_MaxHits{ 3 };
	bool m_CanHit{ true };



	void CheckBoundaries();
	void BulletHit();
};



