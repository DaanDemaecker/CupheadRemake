#pragma once
#include "Vector2f.h"
#include <memory>
class Texture;

class Pyramid final
{
public:
	explicit Pyramid(const Point2f& pos);
	~Pyramid();

	Pyramid(Pyramid& rhs) = delete;
	Pyramid(Pyramid&& rhs) = delete;

	Pyramid& operator=(Pyramid& rhs) = delete;
	Pyramid& operator=(Pyramid&& rhs) = delete;

	void Draw();

	void Update(float elapsedSec);

	bool CheckBulletHit(const Circlef& pos, float damage);
	void CheckPlayerHit(Rectf& shape, Vector2f& velocity);

	std::weak_ptr<Point2f> GetTarget();
private:
	Texture* m_pTexture;
	Rectf m_Shape;
	Circlef m_HitBox;
	std::shared_ptr<Point2f> m_pTarget;

	bool m_Hit{ false };
	float m_HitTimer{};
	const float m_HitTime{ 0.1f };

	float m_Health{20};

	bool m_Destroyed{ false };

	void CheckDeath();
};

