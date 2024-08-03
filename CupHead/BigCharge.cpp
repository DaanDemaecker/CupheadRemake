#include "pch.h"
#include "BigCharge.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

BigCharge::BigCharge(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier)
	:BigBullet(center, angle, damageMultiplier, 60)
{
	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 4, 16, 0, 1 / 15.f);
}

BigCharge::~BigCharge()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigCharge::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, 0); //m_Angle / float(M_PI) * 180);
	}
}

void BigCharge::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		m_pTexture->Update(elapsedSec);
		if (m_pTexture->GetCrFrame() >= 15) m_IsActivated = false;
	}
}


float BigCharge::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

bool BigCharge::Hit()
{
	if (m_Hit)
	{
		return false;
	}
	else
	{
		m_Hit = true;
		return true;
	}

}

Enums::BulletType BigCharge::GetBulletType() const
{
	return Enums::BulletType::Charge;
}
