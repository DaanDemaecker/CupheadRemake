#include "pch.h"
#include "OverworldCamera.h"

OverworldCamera::OverworldCamera(const Rectf& window, const Rectf& boundaries)
	:m_Window{window}, m_Boundaries{boundaries}
{
}

OverworldCamera::~OverworldCamera()
{
}

void OverworldCamera::TranslateCamera(const Point2f& playerPos)
{
	Point2f cameraPos{ TrackPlayer(playerPos) };
	Clamp(cameraPos);

	glTranslatef(-cameraPos.x, -cameraPos.y, 0);
}

Point2f OverworldCamera::TrackPlayer(const Point2f& playerPos)
{
	Point2f cameraPos{};
	cameraPos.x = playerPos.x - m_Window.width / 2;
	cameraPos.y = playerPos.y - m_Window.height / 2;

	return cameraPos;
}

void OverworldCamera::Clamp(Point2f& cameraPos)
{
	if (cameraPos.x < 0)cameraPos.x = 0;
	else if (cameraPos.x > m_Boundaries.width - m_Window.width)cameraPos.x = m_Boundaries.width - m_Window.width;

	if (cameraPos.y < 0)cameraPos.y = 0;
	else if (cameraPos.y > m_Boundaries.height - m_Window.height)cameraPos.y = m_Boundaries.height - m_Window.height;
}
