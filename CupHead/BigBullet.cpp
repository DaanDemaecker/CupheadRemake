#include "pch.h"
#include "BigBullet.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

BigBullet::BigBullet(const Point2f& center, float angle, float damageMultiplier, float size)
	:m_IsActivated{true}, m_HitBox{center, size}, m_StartLocation{center}, m_Angle{angle}, m_DamageMultiplier{damageMultiplier}
{

}

BigBullet::~BigBullet()
{

}

bool BigBullet::IsActivated() const
{
	return m_IsActivated;
}

Circlef BigBullet::GetHitbox() const
{
	return m_HitBox;
}

Enums::BulletType BigBullet::GetBulletType() const
{
	return Enums::BulletType::none;
}

