#include "pch.h"
#include "ObjectManagerTutorial.h"
#include "Object.h"
#include "Pyramid.h"
#include "Bullet.h"
#include "BigBullet.h"
#include "utils.h"
#include "ParticleSoundeffectManager.h"

ObjectManagerTutorial::ObjectManagerTutorial()
{
	m_pSpheres.push_back(new Object{ "./resources/Images/Levels/Tutorial/Sphere_Parry.png", ParticleSoundeffectManager::ParticleType::Null, ParticleSoundeffectManager::SFXType::None,1, 0, false, true});
	m_pSpheres.push_back(new Object{ "./resources/Images/Levels/Tutorial/Sphere.png",ParticleSoundeffectManager::ParticleType::Null, ParticleSoundeffectManager::SFXType::None,1, 0, false, false });
	m_pSpheres.push_back(new Object{ "./resources/Images/Levels/Tutorial/Sphere.png",ParticleSoundeffectManager::ParticleType::Null, ParticleSoundeffectManager::SFXType::None,1, 0, false, false });

	m_pPyramid = new Pyramid{ Point2f{3895, 223} };
	
	m_pTargets.push_back(m_pPyramid->GetTarget());

	m_pPlatforms.push_back(new Platform(Point2f{2135, 368}, Point2f{2468, 368}));

	SetObjects();
}

ObjectManagerTutorial::~ObjectManagerTutorial()
{
	for (int i{}; i < m_pSpheres.size(); i++)
	{
		delete m_pSpheres[i];
		m_pSpheres[i] = nullptr;
	}
	m_pSpheres.clear();
	delete m_pPyramid;
	m_pPyramid = nullptr;
}

void ObjectManagerTutorial::DrawLevel4() const
{
	for (int i{}; i < m_pSpheres.size(); i++)
	{
		m_pSpheres[i]->Draw();
	}
	m_pPyramid->Draw();
}

void ObjectManagerTutorial::Update(float elapsedSec, const Level* level, const Rectf& shape)
{
	for (int i{}; i < m_pSpheres.size(); i++)
	{
		m_pSpheres[i]->Update(elapsedSec, level);
	}
	m_pPyramid->Update(elapsedSec);
}

void ObjectManagerTutorial::SetObjects()
{
	m_pSpheres[0]->SetObject(Point2f{ 4295, 300 }, 0, 0);
	m_pSpheres[1]->SetObject(Point2f{ 4490, 300 }, 0, 0);
	m_pSpheres[2]->SetObject(Point2f{ 4694, 300 }, 0, 0);
}

bool ObjectManagerTutorial::CheckParry(const Rectf& shape, float damage)
{
	int i{};
	bool hit{false};
	while (!hit && i < m_pSpheres.size())
	{
		hit = m_pSpheres[i]->CheckParry(shape);
		i++;
	}

	if (hit)
	{
		Point2f p0{ m_pSpheres[0]->GetCenter() }, p1{ m_pSpheres[1]->GetCenter() }, p2{ m_pSpheres[2]->GetCenter() };
		m_pSpheres[0]->SetObject(p1, 0, 0);
		m_pSpheres[1]->SetObject(p2, 0, 0);
		m_pSpheres[2]->SetObject(p0, 0, 0);
	}

	return hit;
}

bool ObjectManagerTutorial::Checkhits(Rectf& shape, bool parry, Vector2f& velocity)
{
	int i{};
	bool hit{ false };
	while (!hit && i < m_pSpheres.size())
	{
		hit = m_pSpheres[i]->CeckHit(shape, parry);
		i++;
	}
	return hit;
}

void ObjectManagerTutorial::HandleCollisions(Rectf& shape, Vector2f& velocity, bool passingTrough) const
{
	m_pPyramid->CheckPlayerHit(shape, velocity);
	ObjectManager::HandlePlatformCollisions(shape, velocity, passingTrough);
}

float ObjectManagerTutorial::CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets)
{
	float damage{};
	for (int i{}; i < bullets.size(); i++)
	{
		if (bullets[i]->IsActivated())
		{
			if (m_pPyramid->CheckBulletHit(bullets[i]->GetHitbox(), bullets[i]->GetDamage()))
			{
				bullets[i]->Hit();
				damage += bullets[i]->GetDamage();
			}
		}
		
	}

	for (int i{}; i < bigBullets.size(); i++)
	{
		if (bigBullets[i]->IsActivated())
		{
			if (m_pPyramid->CheckBulletHit(bigBullets[i]->GetHitbox(), bigBullets[i]->GetDamage()))
			{
				bigBullets[i]->Hit();
			}
		}
	}
	return damage;
}

LoseScreenData ObjectManagerTutorial::GetLoseScreenData()
{
	return LoseScreenData();
}
