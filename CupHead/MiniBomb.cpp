#include "pch.h"
#include "MiniBomb.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include "OneCycleSpriteSheet.h"

extern ParticleSoundeffectManager* g_pSFXManager;

MiniBomb::MiniBomb(Texture* texturePtr, const Point2f& center, float damageMultiplier)
	:Bullet{ center, 0, damageMultiplier, 15 }
{
	m_Speed = Vector2f{ 800, 0 };
	m_Damage = 23.f;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 7, 0);
}

MiniBomb::~MiniBomb()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void MiniBomb::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, 0);

	}
}

void MiniBomb::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{

		m_pTexture->Update(elapsedSec);
		m_Speed.y += m_Gravity * elapsedSec;

		m_HitBox.center += m_Speed * elapsedSec;

		CheckBoundaries(level);
	}
}

void MiniBomb::Hit()
{
	m_IsActivated = false;
}

float MiniBomb::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void MiniBomb::CheckBoundaries(const Level* level)
{
	if (!level->IsInFrame(m_HitBox.center))
	{
		m_IsActivated = false;
	}

}