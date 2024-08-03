#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2f.h"
#include "utils.h"
#include <string>

class Camera;

class Level
{
public:
	explicit Level(const Rectf& window, const std::string& verticesTextPath);
	virtual ~Level();

	Level(Level& rhs) = delete;
	Level(Level&& rhs) = delete;

	Level& operator=(Level& rhs) = delete;
	Level& operator=(Level&& rhs) = delete;

	virtual void DrawLevel1() const;
	virtual void DrawLevel2() const;
	virtual void DrawLevel3() const;
	virtual void DrawLevel4() const;
	virtual void DrawLevel5() const;

	virtual void UpdateCamera(float elapsedSec, const Rectf& target, float velocityX);

	virtual void UpdateLevel(float elapsedSec, const Rectf& player);
	virtual void TranslateCameraMiddleground() const;
	virtual void TranslateCameraForeground() const;

	virtual Point2f GetCameraDifference() const;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape) const;
	bool IsOnGround(const Point2f& center) const;
	Point2f GetGroundPos(const Rectf& actorShape) const;

	virtual bool IsInFrame(const Point2f& pos) const = 0;
	virtual bool IsInLevel(const Point2f& pos) const;

	Rectf GetBoundaries();

protected:

	const Rectf m_Window;

	std::vector<std::vector<Point2f>> m_Vertices;
	Rectf m_LevelBoundaries;
};

