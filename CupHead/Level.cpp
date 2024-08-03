#include "pch.h"
#include "Level.h"
#include "utils.h"
#include "SVGParser.h"
#include "Camera.h"

Level::Level(const Rectf& window, const std::string& verticesTextPath)
	:m_Window{window}
{
	SVGParser::GetVerticesFromSvgFile(verticesTextPath, m_Vertices);
}

Level::~Level()
{
}

void Level::DrawLevel1() const
{

}

void Level::DrawLevel2() const
{
}

void Level::DrawLevel3() const
{

}

void Level::DrawLevel4() const
{
	
}

void Level::DrawLevel5() const
{
	
}

void Level::UpdateLevel(float elapsedSec, const Rectf& player)
{

}

void Level::TranslateCameraMiddleground() const
{

}

void Level::TranslateCameraForeground() const
{

}

Point2f Level::GetCameraDifference() const
{
	return Point2f();
}


void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	float innerBorder{ 5 }, outerBorder{ 2 };
	Point2f p1{ actorShape.left + actorShape.width / 2, actorShape.bottom + innerBorder }, p2{ p1.x - actorShape.width / 2 - outerBorder, p1.y };

	utils::HitInfo hitinfo{};

#pragma region horizontal
	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.x < 0)
		{
			actorVelocity.x = 0;
			actorShape.left = hitinfo.intersectPoint.x + outerBorder;
		}
	}

	p2.x = p1.x + actorShape.width / 2 + outerBorder;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.x > 0)
		{
			actorVelocity.x = 0;
			actorShape.left = hitinfo.intersectPoint.x - actorShape.width - outerBorder;
		}
	}

	p1.y += actorShape.height - 2 * innerBorder;
	p2.y = p1.y;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.x > 0)
		{
			actorVelocity.x = 0;
			actorShape.left = hitinfo.intersectPoint.x - actorShape.width - outerBorder;
		}
	}

	p2.x = p1.x - actorShape.width / 2 - outerBorder;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.x < 0)
		{
			actorVelocity.x = 0;
			actorShape.left = hitinfo.intersectPoint.x;
		}
	}
#pragma endregion horizontal


#pragma region vertical
	p1 = Point2f{ actorShape.left + innerBorder, actorShape.bottom + actorShape.height / 2 };
	p2 = Point2f{p1.x,  p1.y - actorShape.height / 2 - outerBorder };

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.y < 0)
		{
			actorVelocity.y = 0;
			actorShape.bottom = hitinfo.intersectPoint.y + outerBorder;
		}
	}

	p2.y = p1.y + actorShape.height / 2 + outerBorder;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.y > 0)
		{
			actorVelocity.y = 0;
			actorShape.bottom = hitinfo.intersectPoint.y - actorShape.height - outerBorder;
		}
	}

	p1.x += actorShape.width - 2 * innerBorder;
	p2.x = p1.x;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.y > 0)
		{
			actorVelocity.y = 0;
			actorShape.bottom = hitinfo.intersectPoint.y - actorShape.height - outerBorder;
		}
	}

	p2.y = p1.y - actorShape.height / 2 - outerBorder;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo) && actorVelocity.y < 0)
		{
			actorVelocity.y = 0;
			actorShape.bottom = hitinfo.intersectPoint.y;
		}
	}
#pragma endregion vertical
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	float innerBorder{ 5 }, outerBorder{ 2 };
	Point2f p1{}, p2{};
	utils::HitInfo hitinfo{};

	p1 = Point2f{ actorShape.left + innerBorder, actorShape.bottom + actorShape.height / 2 };
	p2 = Point2f{ p1.x, p1.y - actorShape.height / 2 - outerBorder };

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo))
		{
			return true;
		}
	}

	p1.x += actorShape.width - innerBorder * 2;
	p2.x += actorShape.width - innerBorder * 2;

	for (std::vector<Point2f> vertices : m_Vertices)
	{
		if (utils::Raycast(vertices, p1, p2, hitinfo))
		{
			return true;
		}
	}
	return false;
}

bool Level::IsOnGround(const Point2f& center) const
{
	float outerBorder{ 20};
	Point2f p1, p2{};
	utils::HitInfo hitinfo{};

	p1 = Point2f{ center.x, center.y + outerBorder };
	p2 = Point2f{ center.x, center.y - outerBorder};

	if (utils::Raycast(m_Vertices[0], p1, p2, hitinfo))
	{
		return true;
	}
	return false;
}

Point2f Level::GetGroundPos(const Rectf& actorShape) const
{
	Point2f p1{}, p2{};
	utils::HitInfo hitinfo{};

	p1 = Point2f{ actorShape.left + actorShape.width / 2, actorShape.bottom + 30 };
	p2 = Point2f{ p1.x, m_LevelBoundaries.bottom };

	utils::Raycast(m_Vertices[0], p1, p2, hitinfo);
	return hitinfo.intersectPoint;
}

bool Level::IsInLevel(const Point2f& pos) const
{
	return utils::IsPointInPolygon(pos, m_Vertices[0]);
}

Rectf Level::GetBoundaries()
{
	return m_LevelBoundaries;
}

void Level::UpdateCamera(float elapsedSec, const Rectf& target, float velocityX)
{
}