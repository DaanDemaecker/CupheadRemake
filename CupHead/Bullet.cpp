#include "pch.h"
#include "Bullet.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

Bullet::Bullet(const Point2f& center, float angle, float damageMultiplier, float size)
	:m_IsActivated{ true }, m_HitBox{center, size}, m_StartLocation{center}, m_Angle{angle}, m_DamageMultiplier{damageMultiplier}
{

}

Bullet::~Bullet()
{

}

bool Bullet::IsActivated()
{
	return m_IsActivated;
}

Circlef Bullet::GetHitbox()
{
	return m_HitBox;
}

float Bullet::GetFireRate(Enums::BulletType bulletType)
{
	switch (bulletType)
	{
	case Enums::BulletType::PeaShooter:
		return 2 / 15.f;
		break;
	case Enums::BulletType::SpreadShot:
		return  6.2f/41.33f;
		break;
	case Enums::BulletType::Roundabout:
		return 8.5f / 31.875f;
		break;
	case Enums::BulletType::Lobber:
		return 0.5f;
		break;
	case Enums::BulletType::Chaser:
		return 2.6f / 15.60f;
		break;
	default:
		return 2 / 15.f;
		break;
	}
}

float Bullet::GetFireRate(Enums::PlaneBulletType bulletType)
{
	switch (bulletType)
	{
	case Enums::PlaneBulletType::PeaShooter:
		return 4 / 40.f;
		break;
	case Enums::PlaneBulletType::Mini_Bomb:
		return 23 / 37.30f;
		break;
	case Enums::PlaneBulletType::Chalice_PeaShooter:
		return 10.95f / 44;
		break;
	case Enums::PlaneBulletType::Chalice_Bomb:
		return 9.1f / 42.f;
		break;
	case Enums::PlaneBulletType::Mini:
		return 2 / 20.f;
	default:
		return 2 / 15.f;
		break;
	}
}