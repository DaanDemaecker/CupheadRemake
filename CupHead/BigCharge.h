#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;
#include "Vector2f.h"


class BigCharge final : public BigBullet
{
public:
	explicit BigCharge(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~BigCharge();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual float GetDamage() const override;

	virtual bool Hit() override;

	virtual Enums::BulletType GetBulletType() const override;
private:
	BulletSpriteSheet* m_pTexture;

	float m_Damage{ 27.f };

	bool m_Hit{ false };
};



