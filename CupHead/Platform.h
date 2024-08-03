#pragma once
#include <vector>

class Platform
{
public:
	Platform(const Point2f& p1, const Point2f& p2);

	~Platform();

	std::vector<Point2f> GetVertices();

private:
	Point2f m_P1;
	Point2f m_P2;
};

