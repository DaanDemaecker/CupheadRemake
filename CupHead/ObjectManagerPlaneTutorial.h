#pragma once
#include <vector>
#include "Vector2f.h"
#include "ObjectManager.h"
class Object;
class Level;
class Pyramid;
class Bullet;
class BigBullet;

class ObjectManagerPlaneTutorial final : public ObjectManager
{
public:
	ObjectManagerPlaneTutorial();
	virtual ~ObjectManagerPlaneTutorial() override;

	virtual void DrawLevel1() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& shape) override;

	virtual bool CheckParry(const Rectf& shape, float damage) override;
	virtual bool Checkhits(Rectf& shape, bool parry, Vector2f& velocity) override;

	virtual LoseScreenData GetLoseScreenData() override;
private:
	Object* m_pSphere;

	Point2f m_SpherePos1{884, 319};
	Point2f m_SpherePos2{1027, 317};

	bool m_FirstSpherePos{ true };
};

