#include "pch.h"
#include "Platform.h"

Platform::Platform(const Point2f& p1, const Point2f& p2)
	:m_P1{p1}, m_P2{p2}
{
}

Platform::~Platform()
{
}

std::vector<Point2f> Platform::GetVertices()
{
	return std::vector<Point2f>{m_P1, m_P2};
}
