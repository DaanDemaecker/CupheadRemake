#include "pch.h"
#include "Shadow.h"
#include "utils.h"
#include "Texture.h"
#include <iostream>

Shadow::Shadow()
	:m_NrRows{ 3 }, m_NrCols{ 4 }, m_NrFrames{ 11 }, m_MaxDistance{ 250 }
{
	m_pTexture = new Texture{ "./resources/Images/player/CupHead/Shadow.png" };
}

Shadow::~Shadow()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Shadow::Draw() const
{
	Rectf srcRect{};
	utils::CalculateSrcRect(m_pTexture, srcRect, m_NrCols, m_NrRows, m_Frame);

	m_pTexture->Draw(Point2f{ m_GroundPos.x - srcRect.width / 2, m_GroundPos.y + 5 }, srcRect);
}

void Shadow::Update(const Point2f& playerPos, const Point2f& groundPos)
{
	m_GroundPos = groundPos;

	float distance{ utils::GetDistance(playerPos, groundPos) };

	if (distance >= m_MaxDistance)m_Frame = m_NrFrames;
	else
	{
		m_Frame = int((distance / m_MaxDistance) * m_NrFrames);
	}
}
