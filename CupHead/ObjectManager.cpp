#include "pch.h"
#include "ObjectManager.h"
#include "utils.h"
#include "Level.h"
#include <iostream>

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	for (int i{}; i < m_pPlatforms.size(); i++)
	{
		delete m_pPlatforms[i];
		m_pPlatforms[i] = nullptr;
	}
	m_pPlatforms.clear();
}

void ObjectManager::DrawLevel1() const
{

}

void ObjectManager::DrawLevel2() const
{

}

void ObjectManager::DrawLevel3() const
{

}

void ObjectManager::DrawLevel4() const
{

}

void ObjectManager::DrawLevel5() const
{

}

void ObjectManager::FixParallax(const Point2f& cameraDifference)
{
}

void ObjectManager::HandleCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough) const
{
}

float ObjectManager::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	return 0.0f;
}

std::weak_ptr<Point2f> ObjectManager::GetClosestTarget(const Point2f& bulletPos, const Level* level)
{
	ClearExpiredTargets();

	if (m_pTargets.size() > 0)
	{

		float shortestDistance{utils::GetDistance(bulletPos, *m_pTargets[0].lock())};
		int shortestDistanceIndex{0};

		for (int i{}; i < m_pTargets.size(); i++)
		{
			Point2f target{ *m_pTargets[i].lock() };
			float distance{ utils::GetDistance(bulletPos, target )};
			if (distance < shortestDistance)
			{
				shortestDistance = distance;
				shortestDistanceIndex = i;
			}
		}
		if(level->IsInFrame(*m_pTargets[shortestDistanceIndex].lock()))return m_pTargets[shortestDistanceIndex];
		else return std::weak_ptr<Point2f>();
	}
	else return std::weak_ptr<Point2f>();
}

void ObjectManager::HandlePlatformCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough) const
{
	float innerBorder{ 5 }, outerBorder{ 2 };
	Point2f p1{}, p2{}, p3{}, p4{};
	utils::HitInfo hitinfo{};

	p1 = Point2f{ shape.left + innerBorder, shape.bottom + innerBorder };
	p2 = Point2f{ p1.x, shape.bottom - outerBorder };
	p3 = Point2f{ p1.x + shape.width - innerBorder * 2, p1.y };
	p4 = Point2f{ p2.x + shape.width - innerBorder * 2, p2.y };

	for (int i{}; i < m_pPlatforms.size(); i++)
	{
		if (!passingTrough)
		{
			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p1, p2, hitinfo) && velocity.y < 0)
			{
				velocity.y = 0;
				shape.bottom = hitinfo.intersectPoint.y + outerBorder;
			}

			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p3, p4, hitinfo) && velocity.y < 0)
			{
				velocity.y = 0;
				shape.bottom = hitinfo.intersectPoint.y + outerBorder;
			}
		}
		else if(m_LatestPlatform != i)
		{
			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p1, p2, hitinfo) && velocity.y < 0)
			{
				velocity.y = 0;
				shape.bottom = hitinfo.intersectPoint.y + outerBorder;
			}

			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p3, p4, hitinfo) && velocity.y < 0)
			{
				velocity.y = 0;
				shape.bottom = hitinfo.intersectPoint.y + outerBorder;
			}
		}
	}
}

bool ObjectManager::IsOnPlatform(Rectf& shape, bool passingTrough)
{
	float innerBorder{ 15 }, outerBorder{ 3 };
	Point2f p1{}, p2{}, p3{}, p4{};
	utils::HitInfo hitinfo{};

	p1 = Point2f{ shape.left + innerBorder, shape.bottom + innerBorder };
	p2 = Point2f{ p1.x, shape.bottom - outerBorder };
	p3 = Point2f{ p1.x + shape.width - innerBorder * 2, p1.y };
	p4 = Point2f{ p2.x + shape.width - innerBorder * 2, p2.y };

	for (int i{}; i < m_pPlatforms.size(); i++)
	{
		if (!passingTrough)
		{
			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p1, p2, hitinfo))
			{
				m_LatestPlatform = i;
				return true;
			}

			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p3, p4, hitinfo))
			{
				m_LatestPlatform = i;
				return true;
			}
		}
		else if (m_LatestPlatform != i)
		{
			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p1, p2, hitinfo))
			{
				m_LatestPlatform = i;
				return true;
			}

			if (utils::Raycast(m_pPlatforms[i]->GetVertices(), p3, p4, hitinfo))
			{
				m_LatestPlatform = i;
				return true;
			}
		}
	}

	return false;
}

void ObjectManager::ClearExpiredTargets()
{
	int targetsAmount{ int(m_pTargets.size()) };
	for (int i{}; i < targetsAmount; i++)
	{
		if (m_pTargets[i].expired())
		{
			if (targetsAmount-- > 0)
			{
				m_pTargets[i] = m_pTargets[m_pTargets.size() - 1];
				m_pTargets.pop_back();
				i--;
			}
			else
			{
				m_pTargets.clear();
				break;
			}
		}
	}
}
