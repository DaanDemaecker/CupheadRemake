#pragma once
#include "Vector2f.h"
#include "Enums.h"
#include <map>
#include <vector>

class Texture;
class OverworldCamera;
class SpriteSheet;

class OverworldIsle
{
public:
	OverworldIsle(const Rectf& window);
	~OverworldIsle();

	OverworldIsle(OverworldIsle& rhs) = delete;
	OverworldIsle(OverworldIsle&& rhs) = delete;

	OverworldIsle operator=(OverworldIsle& rhs) = delete;
	OverworldIsle operator=(OverworldIsle&& rhs) = delete;

	void TranslateCamera(const Point2f& playerPos);

	void HandleCollisions(Point2f& playerPos, Vector2f& velocity);

	void Update(float elapsedSec, const Point2f& playerPos);

	void DrawBase(const Point2f& playerPos) const;

	void DrawTop(const Point2f& playerPos) const;

	bool IsAvatarNearBuilding();

	Enums::Buildings GetLatestBuilding();

private:
	std::vector<std::vector<Point2f>> m_Vertices;

	std::map<Enums::Buildings, OverworldBuilding*> m_pBuildings;

	std::vector<OverworldObstacle*> m_pObstacles;

	Texture* m_pOceanTexture;

	SpriteSheet* m_pRipples;

	Texture* m_pBaseTexture;

	Texture* m_pTopTexture;

	OverworldCamera* m_pCamera;

	bool m_IsAvatarNearBuilding{ false };

	Enums::Buildings m_LatestBuilding{Enums::Buildings::Tutorial};
};

