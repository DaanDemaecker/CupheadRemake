#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class SpriteSheet;
class BulletSpriteSheet;
class Texture;


class BigChaser final : public BigBullet
{
public:
	explicit BigChaser(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, int rank);
	virtual ~BigChaser();

	static void UpdateAngle(float elapsedSec);

	void Disconnect();

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;

	virtual Enums::BulletType GetBulletType() const override;

private:
	BulletSpriteSheet* m_pTexture;

	SpriteSheet* m_pTrail;

	const float m_Speed{ 1000 };

	const float m_Damage{ 2.6f };

	float m_Timer{ 1.f };

	const float m_RotSpeed{ float(M_PI) * 2.5f };
	bool m_TargetExpired{ false };

	bool m_LockedOnTarget{false};

	int m_Rank;

	Point2f m_Target;

	static float m_CirlceAngle;

	void BulletHit();
};



