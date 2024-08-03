#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include <iostream>

Camera::Camera(const Rectf& window)
	:m_Window{window}, m_CameraPos{-75.f, 0.f}, m_Speed{},
	m_LevelBoundaries{Rectf{0, 0, window.width, window.height}}
{

}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::TranslateMiddleground(float factor) const
{
	glTranslatef(-m_CameraPos.x / factor, 0, 0);
}

void Camera::TranslateForeground(float yOffset) const
{
	glTranslatef(-m_CameraPos.x, yOffset, 0);
}

void Camera::Update(float elapsedSec, const Rectf& target, float velocityX)
{
	float fraction{ 4.5f / 10.f };
	float posRatio{ (target.left - m_CameraPos.x) / m_Window.width };

	if (posRatio <= fraction / 2)
	{
		m_Speed -= m_Acceleration * elapsedSec;
	}
	else if (posRatio <= fraction)
	{
		if (velocityX < 0 && m_Speed > -m_MaxSpeed)m_Speed -= m_Acceleration * elapsedSec;
		else if (velocityX >= 0 && m_Speed < 0) m_Speed += m_Acceleration / 2 * elapsedSec;
	}
	else if (posRatio >= 1 - fraction / 2)
	{
		m_Speed += m_Acceleration * elapsedSec;
	}
	else if (posRatio >= 1-fraction)
	{
		if (velocityX > 0 && m_Speed < m_MaxSpeed)m_Speed += m_Acceleration * elapsedSec;
		else if (velocityX <= 0 && m_Speed > 0) m_Speed -= m_Acceleration / 2 * elapsedSec;
	}

	if(posRatio > fraction && velocityX <= 0 && m_Speed > 0)m_Speed -= m_Acceleration / 2 * elapsedSec;
	else if (posRatio < 1-fraction && velocityX >= 0 && m_Speed < 0)m_Speed += m_Acceleration / 2 * elapsedSec;

	if (m_Speed < -m_MaxSpeed)m_Speed = -m_MaxSpeed;
	else if (m_Speed > m_MaxSpeed)m_Speed = m_MaxSpeed;

	m_CameraPos.x += m_Speed * elapsedSec;

	ClampOnPlayer(m_CameraPos, target);
	ClampOnLevel(m_CameraPos, target);
}

void Camera::ClampOnPlayer(Point2f& cameraPos, const Rectf& target)
{
	const float fraction{ 2/10.f };

	if (cameraPos.x < target.left - m_Window.width*(1-fraction))
	{
		cameraPos.x = target.left - m_Window.width * (1 - fraction);
	}
	else if (cameraPos.x > target.left - m_Window.width * fraction)
	{
		cameraPos.x = target.left - m_Window.width * fraction;
	}
}

void Camera::ClampOnLevel(Point2f& cameraPos, const Rectf& target)
{
	if (cameraPos.x < m_LevelBoundaries.left)
	{
		cameraPos.x = m_LevelBoundaries.left;
	}
	else if (cameraPos.x + m_Window.width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		cameraPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Window.width;
	}
}

Point2f Camera::GetCameraPos()
{
	return m_CameraPos;
}