#include "pch.h"
#include "ChaliceBomb.h"
#include "BulletSpriteSheet.h"
#include "Level.h"
#include "ParticleSoundeffectManager.h"
#include "OneCycleSpriteSheet.h"

extern ParticleSoundeffectManager* g_pSFXManager;

ChaliceBomb::ChaliceBomb(Texture* texturePtr, const Point2f& center, float damageMultiplier)
	:Bullet{ center, 0, damageMultiplier, 15 }
{
	m_Speed = Vector2f{ 800, 0 };
	m_Damage = 9.1f;

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 0);
}

ChaliceBomb::~ChaliceBomb()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void ChaliceBomb::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, 0);

	}
}

void ChaliceBomb::Update(float elapsedSec, const Level* level)
{
	if (m_IsActivated)
	{

		m_pTexture->Update(elapsedSec);
		m_Speed.y += m_Gravity * elapsedSec;

		m_HitBox.center.x += m_Speed.x * elapsedSec;
		m_HitBox.center.y += m_Speed.y * elapsedSec;

		CheckBoundaries(level);
	}
}

void ChaliceBomb::Hit()
{
	m_IsActivated = false;
}

float ChaliceBomb::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

void ChaliceBomb::CheckBoundaries(const Level* level)
{
	if (!level->IsInFrame(m_HitBox.center))
	{
		m_IsActivated = false;
	}

}