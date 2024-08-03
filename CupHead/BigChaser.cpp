#include "pch.h"
#include "BigChaser.h"
#include "utils.h"
#include "Level.h"
#include "BulletSpriteSheet.h"
#include "ParticleSoundeffectManager.h"
#include "Texture.h"
#include "SpriteSheet.h"
extern ParticleSoundeffectManager* g_pSFXManager;

float BigChaser::m_CirlceAngle{};

void BigChaser::UpdateAngle(float elapsedSec)
{
	m_CirlceAngle += elapsedSec * float(M_PI) * 4;

	if (m_CirlceAngle >= 2 * M_PI)m_CirlceAngle -= 2 * float(M_PI);

}

void BigChaser::Disconnect()
{
	m_TargetExpired = true;
	m_LockedOnTarget = false;
}

BigChaser::BigChaser(Texture* texturePtr, const Point2f& center, float angle, float damageMultiplier, int rank)
	:BigBullet(center, angle, damageMultiplier, 60), m_Rank{rank}
{
	m_pTrail = new SpriteSheet{ "./resources/Images/Player/Bullets/Chaser/Chaser_Trail.png", 6, 2, 12 };

	m_pTexture = new BulletSpriteSheet(texturePtr, 4, 2, 8, 1, 1 / 15.f);
}



BigChaser::~BigChaser()
{
	delete m_pTrail;
	m_pTrail = nullptr;
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BigChaser::Draw() const
{
	if (m_IsActivated)
	{
		m_pTexture->Draw(m_HitBox.center, m_CirlceAngle / float(M_PI) * 180);
		m_pTrail->DrawCenter(Point2f{ m_HitBox.center.x - 30 * cosf(m_Angle), m_HitBox.center.y - 30 * sinf(m_Angle) });
	}
}

void BigChaser::Update(float elapsedSec, const Level* level, const Rectf& player)
{
	if (m_IsActivated)
	{
		if (m_Timer > 0)m_Timer -= elapsedSec;

		float distance{ 65 };
		float angle{ float(m_CirlceAngle + (M_PI / 2) * m_Rank )};
		m_Target = Point2f{player.left + player.width/2, player.bottom + player.height/2};
		m_Target.x += distance * cosf(angle);
		m_Target.y += distance * sinf(angle);

		if (m_LockedOnTarget)
		{
			m_HitBox.center = m_Target;
			m_Angle = angle + float(M_PI) / 2.f;
		}
		else
		{
			if (!m_TargetExpired)
			{
				utils::RotateObjectToTarget(m_HitBox.center, m_Angle, m_Target, m_RotSpeed * elapsedSec);
				if (utils::GetDistance(m_Target, m_HitBox.center) <= 80 && m_Timer <= 0)m_LockedOnTarget = true;
			}
			m_HitBox.center.x += cosf(m_Angle) * m_Speed * elapsedSec;
			m_HitBox.center.y += sinf(m_Angle) * m_Speed * elapsedSec;
		}
		m_pTexture->Update(elapsedSec);
		m_pTrail->Update(elapsedSec);
	}
}


void BigChaser::BulletHit()
{
	m_IsActivated = false;
	g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Chaser_Special_Death);
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chaser_Death);
}


float BigChaser::GetDamage() const
{
	return m_Damage * m_DamageMultiplier;
}

Enums::BulletType BigChaser::GetBulletType() const
{
	return Enums::BulletType::Chaser;
}

bool BigChaser::Hit()
{
	BulletHit();
	return true;
}