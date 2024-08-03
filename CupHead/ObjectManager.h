#pragma once
#include <vector>
class Bullet;
class Level;
class BigBullet;
#include "Platform.h"
#include "Vector2f.h"
#include <memory>
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	virtual ~ObjectManager();

	ObjectManager(ObjectManager& rhs) = delete;
	ObjectManager(ObjectManager&& rhs) = delete;

	ObjectManager& operator=(ObjectManager& rhs) = delete;
	ObjectManager& operator=(ObjectManager&& rhs) = delete;


	virtual void DrawLevel1() const;
	virtual void DrawLevel2() const;
	virtual void DrawLevel3() const;
	virtual void DrawLevel4() const;
	virtual void DrawLevel5() const;


	virtual void Update(float elapsedSec, const Level* level, const Rectf& shape) = 0;

	virtual void FixParallax(const Point2f& cameraDifference);

	virtual bool CheckParry(const Rectf& shape, float damage) = 0;
	virtual bool Checkhits(Rectf& shape, bool parry, Vector2f& velocity) = 0;

	virtual void HandleCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough)const;

	virtual float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets);
	std::weak_ptr<Point2f> GetClosestTarget(const Point2f& bulletPos, const Level* level);
	
	void HandlePlatformCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough) const;
	bool IsOnPlatform(Rectf& shape, bool passingTrough);

	virtual LoseScreenData GetLoseScreenData() = 0;

protected:
	std::vector<std::weak_ptr<Point2f>> m_pTargets;

	std::vector<Platform*> m_pPlatforms;
	int m_LatestPlatform{};

private:
	void ClearExpiredTargets();
};

