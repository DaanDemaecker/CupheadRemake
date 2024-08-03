#include "pch.h"
#include "ObjectManagerPlaneTutorial.h"
#include "Object.h"
#include "Pyramid.h"
#include "Bullet.h"
#include "BigBullet.h"
#include "utils.h"
#include "ParticleSoundeffectManager.h"

ObjectManagerPlaneTutorial::ObjectManagerPlaneTutorial()
{
	m_pSphere = new Object{ "./resources/Images/Levels/PlaneTutorial/Sphere.png", ParticleSoundeffectManager::ParticleType::Null, ParticleSoundeffectManager::SFXType::None,1, 0, false, true };

	m_pSphere->SetObject(m_SpherePos1, 0, 0);
}

ObjectManagerPlaneTutorial::~ObjectManagerPlaneTutorial()
{
	delete m_pSphere;
	m_pSphere = nullptr;
}

void ObjectManagerPlaneTutorial::DrawLevel1() const
{
	m_pSphere->Draw();
}

void ObjectManagerPlaneTutorial::Update(float elapsedSec, const Level* level, const Rectf& shape)
{
	
}

bool ObjectManagerPlaneTutorial::CheckParry(const Rectf& shape, float damage)
{
	if (m_pSphere->CheckParry(shape))
	{
		if (m_FirstSpherePos)
		{
			m_FirstSpherePos = false;
			m_pSphere->SetObject(m_SpherePos2, 0, 0);
		}
		else
		{
			m_FirstSpherePos = true;
			m_pSphere->SetObject(m_SpherePos1, 0, 0);
		}
		return true;
	}

	return false;
}

bool ObjectManagerPlaneTutorial::Checkhits(Rectf& shape, bool parry, Vector2f& velocity)
{
	return false;
}

LoseScreenData ObjectManagerPlaneTutorial::GetLoseScreenData()
{
	return LoseScreenData();
}
