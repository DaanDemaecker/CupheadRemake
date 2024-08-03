#pragma once
#include <vector>
#include "Vector2f.h"
#include "ObjectManager.h"
class Object;
class Level;
class Pyramid;
class Bullet;
class BigBullet;

class ObjectManagerTutorial final: public ObjectManager
{
public:
	ObjectManagerTutorial();
	virtual ~ObjectManagerTutorial() override;

	virtual void DrawLevel4() const override;
	virtual void Update(float elapsedSec, const Level* level, const Rectf& shape) override;

	void SetObjects();

	virtual bool CheckParry(const Rectf& shape, float damage) override;
	virtual bool Checkhits(Rectf& shape, bool parry, Vector2f& velocity) override;

	virtual void HandleCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough) const override;

	virtual float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets) override;

	virtual LoseScreenData GetLoseScreenData() override;
private:
	std::vector<Object*> m_pSpheres;
	Pyramid* m_pPyramid;
};

