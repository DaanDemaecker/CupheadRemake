#pragma once
#include "Bullet.h"
class Texture;
class BulletSpriteSheet;
class OneCycleSpriteSheet;
class Level;
#include "Vector2f.h"

class Lobber final : public Bullet
{
public:
	explicit Lobber(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier);
	virtual ~Lobber() override;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const Level* level) override;

	virtual void Hit() override;

	virtual float GetDamage() const override;
private:
	enum class State
	{
		flying,
		bounce1,
		bounce2
	};
	State m_State{ State::flying };

	BulletSpriteSheet* m_pTexture;

	void CheckBoundaries(const Level* level);

	OneCycleSpriteSheet* m_pBounce1;
	OneCycleSpriteSheet* m_pBounce2;


	float m_Damage;
	Vector2f m_Speed;
	const float m_MaxSpeed{ 1500.f };
	const float m_Gravity{ -2700 };

};

