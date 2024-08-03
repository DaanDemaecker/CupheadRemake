#include "pch.h"
#include "OverworldIsle.h"
#include "Texture.h"
#include "OverworldCamera.h"
#include "SpriteSheet.h"
#include "SVGParser.h"

#include "utils.h"
#include <string>

OverworldIsle::OverworldIsle(const Rectf& window)
{
	std::string textPath{ "./resources/images/overworld/world_1/" };

	SVGParser::GetVerticesFromSvgFile(textPath + "Vertices.svg", m_Vertices);

	m_pOceanTexture = new Texture{ textPath + "Ocean.png" };
	m_pRipples = new SpriteSheet{ textPath + "Ripples.png", 1, 6, 6, 1 / 10.f };
	m_pBaseTexture = new Texture{ textPath + "Base.png" };
	m_pTopTexture = new Texture{ textPath + "Top.png" };

	m_pCamera = new OverworldCamera{ window, Rectf{0, 0, m_pOceanTexture->GetWidth(), m_pOceanTexture->GetHeight()} };

	textPath = "./resources/images/overworld/world_1/Buildings/";
	m_pBuildings[Enums::Buildings::Tutorial] = new OverworldBuilding{ textPath + "Tutorial.png", 3,  Point2f{ 778, 1370 }, 1500 };
	m_pBuildings[Enums::Buildings::ShmupTutorial] = new OverworldBuilding{ textPath + "ShmupTutorial.png", 3,  Point2f{ 2860, 1985 }, 2100 };

	m_pBuildings[Enums::Buildings::BotanicPanic] = new OverworldBuilding{ textPath + "BotanicPanic.png", 3,  Point2f{ 1807, 633 }, 730};
	m_pBuildings[Enums::Buildings::ClipJointCalammity] = new OverworldBuilding{ textPath + "ClipJointCalamity.png", 3,  Point2f{ 2213, 217 }, 200 };
	m_pBuildings[Enums::Buildings::FloralFury] = new OverworldBuilding{ textPath + "FloralFury.png", 3,  Point2f{ 3672, 1358 }, 1500 };
	m_pBuildings[Enums::Buildings::RuseOfAnOoze] = new OverworldBuilding{ textPath + "RuseOfAnOoze.png", 3,  Point2f{ 2226, 1493 }, 1600 };
	m_pBuildings[Enums::Buildings::ThreateninZeppelin] = new OverworldBuilding{ textPath + "ThreateninZeppelin.png", 3,  Point2f{ 3355, 1873 }, 2100 };
	
	m_pBuildings[Enums::Buildings::ForestFollies] = new OverworldBuilding{ textPath + "RNG.png", 1,  Point2f{ 2832, 1495 }, 1800 };
	m_pBuildings[Enums::Buildings::TreetopTrouble] = new OverworldBuilding{ textPath + "RNG.png", 1,  Point2f{ 2063, 1352 }, 1800 };

	m_pBuildings[Enums::Buildings::DieHouse] = new OverworldBuilding{ textPath + "DieHouse.png", 3,  Point2f{ 3910, 900 }, 955 };
	m_pBuildings[Enums::Buildings::Shop] = new OverworldBuilding{ textPath + "Shop.png", 3,  Point2f{ 2281, 1010 }, 1154 };
	m_pBuildings[Enums::Buildings::Mausoleum] = new OverworldBuilding{ textPath + "Mausoleum.png", 3,  Point2f{ 3292, 482 }, 700 };


	textPath = "./resources/images/overworld/world_1/obstacles/";
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Bridge_1.png", Point2f{2878, 1668} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Bridge_2.png", Point2f{2259, 1235} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Bridge_3.png", Point2f{2578, 700} });

	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Stair_1.png", Point2f{1979, 1650} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Stair_2.png", Point2f{3219, 1673} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Stair_3.png", Point2f{2665, 1361} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Stair_4.png", Point2f{3375, 1092} });
	m_pObstacles.push_back(new OverworldObstacle{ true, textPath + "Stair_5.png", Point2f{2065, 603} });

}

OverworldIsle::~OverworldIsle()
{
	delete m_pOceanTexture;
	m_pOceanTexture = nullptr;

	delete m_pRipples;
	m_pRipples = nullptr;

	delete m_pBaseTexture;
	m_pBaseTexture = nullptr;

	delete m_pTopTexture;
	m_pTopTexture = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	for (std::pair<Enums::Buildings, OverworldBuilding*> pair: m_pBuildings)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pBuildings.clear();

	for (int i{}; i < m_pObstacles.size(); i++)
	{
		delete m_pObstacles[i];
		m_pObstacles[i] = nullptr;
	}
	m_pObstacles.clear();
}

void OverworldIsle::TranslateCamera(const Point2f& playerPos)
{
	m_pCamera->TranslateCamera(playerPos);
}

void OverworldIsle::HandleCollisions(Point2f& playerPos, Vector2f& velocity)
{
	utils::HitInfo hitInfo{};

	const float checkdistance{ 10 };

	Point2f p1{ playerPos.x, playerPos.y + checkdistance }, p2{ playerPos.x + checkdistance, playerPos.y },
		p3{ playerPos.x, playerPos.y - checkdistance }, p4{ playerPos.x - checkdistance, playerPos.y };

	for (int i{}; i < m_Vertices.size(); i++)
	{
		if (utils::Raycast(m_Vertices[i], playerPos, p1, hitInfo) && velocity.y > 0)
		{
			velocity.y = 0;
			playerPos.y = hitInfo.intersectPoint.y - 5;
		}

		if (utils::Raycast(m_Vertices[i], playerPos, p2, hitInfo) && velocity.x > 0)
		{
			velocity.x = 0;
			playerPos.x = hitInfo.intersectPoint.x - 5;
		}

		if (utils::Raycast(m_Vertices[i], playerPos, p3, hitInfo) && velocity.y < 0)
		{
			velocity.y = 0;
			playerPos.y = hitInfo.intersectPoint.y + 5;
		}

		if (utils::Raycast(m_Vertices[i], playerPos, p4, hitInfo) && velocity.x < 0)
		{
			velocity.x = 0;
			playerPos.x = hitInfo.intersectPoint.x + 5;
		}
	}
}

void OverworldIsle::Update(float elapsedSec, const Point2f& playerPos)
{
	m_pRipples->Update(elapsedSec);


	m_IsAvatarNearBuilding = false;

	for (std::pair<Enums::Buildings, OverworldBuilding*> pair : m_pBuildings)
	{
		pair.second->pSpritesheet->Update(elapsedSec);
		
		if (!m_IsAvatarNearBuilding)
		{
			if (utils::IsPointInEllipse(playerPos, pair.second->hitBox))
			{
				m_IsAvatarNearBuilding = true;
				m_LatestBuilding = pair.first;
			}
		}
	}
}

void OverworldIsle::DrawBase(const Point2f& playerPos) const
{
	m_pOceanTexture->Draw();

	m_pRipples->DrawBottom(Point2f{ m_pRipples->GetFrameWidth() / 2, 0 });

	m_pBaseTexture->Draw();


	for (int i{}; i < m_pObstacles.size(); i++)
	{
		if (m_pObstacles[i]->unLocked)m_pObstacles[i]->pTexture->Draw(m_pObstacles[i]->pos);
	}


	for (std::pair<Enums::Buildings, OverworldBuilding*> pair : m_pBuildings)
	{
		if (playerPos.y <= pair.second->height)pair.second->pSpritesheet->DrawBottom(pair.second->pos);
	}
}

void OverworldIsle::DrawTop(const Point2f& playerPos) const
{
	for (std::pair<Enums::Buildings, OverworldBuilding*> pair : m_pBuildings)
	{
		if (playerPos.y > pair.second->height)pair.second->pSpritesheet->DrawBottom(pair.second->pos);
	}


	m_pTopTexture->Draw();
	
	/*utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	for (int i{}; i < m_Vertices.size(); i++)
	{
		utils::DrawPolygon(m_Vertices[i]);
	}*/
}

bool OverworldIsle::IsAvatarNearBuilding()
{
	return m_IsAvatarNearBuilding;
}

Enums::Buildings OverworldIsle::GetLatestBuilding()
{
	return m_LatestBuilding;
}
