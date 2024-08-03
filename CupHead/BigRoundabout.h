#pragma once
#include "Vector2f.h"
#include <string>
#include "BigBullet.h"
class Level;
class BulletSpriteSheet;
class Texture;
#include <deque>


class BigRoundabout final : public BigBullet
{
public:
	explicit BigRoundabout(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~BigRoundabout();


	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& player) override;

	virtual bool Hit() override;

	virtual float GetDamage() const override;

	virtual Enums::BulletType GetBulletType() const override;


private:
	BulletSpriteSheet* m_pTexture;

	std::deque<Point2f> m_Trail;
	std::deque<Point2f> m_Trail2;
	const float m_TrailSize{ 5 };
	const float m_TrailTime{ .02f };
	float m_TrailTimer{};

	const float m_MiddleSpeed{ 1000 };
	float m_Speed{ 1000 };

	const float m_Acceleration{ 2500 };

	const float m_Damage{ 5.f };

	bool m_Direction{true};

	float m_HitTimer{};
	const float m_HitDelay{ .15f };
	int m_Hits{};
	const int m_MaxHits{ 7 };
	bool m_CanHit{ true };

	bool m_NearZero{ false };

	void BulletHit();
	void CheckTurn(const Rectf& player);

	void DrawTrail() const;
};



