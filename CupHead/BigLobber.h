#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;
#include "Vector2f.h"


class BigLobber final : public BigBullet
{
public:
	explicit BigLobber(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~BigLobber();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;

	virtual Enums::BulletType GetBulletType() const override;

private:
	BulletSpriteSheet* m_pTexture;

	Vector2f m_Speed;
	const float m_Gravity{ -2700 };

	const float m_Damage{ 28.f };

	float m_TrailTimer{};
	float m_TrailTime{ 0.1f };

	void CheckBoundaries();
	void BulletHit();
};



