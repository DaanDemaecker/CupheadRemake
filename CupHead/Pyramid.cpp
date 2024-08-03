#include "pch.h"
#include "Pyramid.h"
#include "Texture.h"
#include "utils.h"
#include "ParticleSoundeffectManager.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

Pyramid::Pyramid(const Point2f& pos)
{
	m_pTexture = new Texture{ "./resources/Images/Levels/Tutorial/Pyramid.png" };
	m_Shape = Rectf{ pos.x, pos.y,m_pTexture->GetWidth() ,m_pTexture->GetHeight() };
	m_HitBox = Circlef{ Point2f{ pos.x + m_Shape.width / 2, pos.y + 3 * m_Shape.height / 4 }, m_Shape.width / 4 };
	m_pTarget = std::make_shared<Point2f>(m_HitBox.center);
}

Pyramid::~Pyramid()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Pyramid::Draw()
{
	if (!m_Destroyed)
	{
		m_pTexture->Draw(m_Shape);
		if (m_Hit)
		{
			utils::SetColor(Color4f{ 0.f, 1.f, 1.f, 0.7f });
			m_pTexture->Draw(m_Shape, Rectf(0, m_pTexture->GetHeight(), m_pTexture->GetWidth(), m_pTexture->GetHeight()), 1);
		}
	}
	
}

void Pyramid::Update(float elapsedSec)
{
	if (m_Hit)
	{
		m_HitTimer += elapsedSec;
		if (m_HitTimer >= m_HitTime)
		{
			m_Hit = false;
			m_HitTimer;
		}
	}
}

bool Pyramid::CheckBulletHit(const Circlef& pos, float damage)
{
	if (!m_Destroyed)
	{
		if (utils::IsOverlapping(pos, m_HitBox))
		{
			m_Hit = true;
			m_Health -= damage;
			CheckDeath();
			return true;
		}
	}
	
	return false;
}

void Pyramid::CheckDeath()
{
	if((m_Health <= 0) && !m_Destroyed)
	{
		m_Destroyed = true;
		m_pTarget.reset();
		g_pSFXManager->CreateParticle(m_HitBox.center, ParticleSoundeffectManager::ParticleType::Explosion);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Explosion);
	}
}

void Pyramid::CheckPlayerHit(Rectf& shape, Vector2f& velocity)
{
	if (!m_Destroyed)
	{
		std::vector<Point2f> vertices;
		vertices.push_back(Point2f{ m_Shape.left, m_Shape.bottom });
		vertices.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom });
		vertices.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height });
		vertices.push_back(Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height });


		float innerBorder{ 5 }, outerBorder{ 2 };
		Point2f p1{ shape.left + shape.width / 2, shape.bottom + innerBorder }, p2{ p1.x - shape.width / 2 - outerBorder, p1.y };

		utils::HitInfo hitinfo{};

		if (utils::Raycast(vertices, p1, p2, hitinfo) && velocity.x < 0)
		{
			velocity.x = 0;
			shape.left = hitinfo.intersectPoint.x + outerBorder;
		}

		p2.x = p1.x + shape.width / 2 + outerBorder;

		if (utils::Raycast(vertices, p1, p2, hitinfo) && velocity.x > 0)
		{
			velocity.x = 0;
			shape.left = hitinfo.intersectPoint.x - shape.width - outerBorder;
		}

		p1.y += shape.height - 2 * innerBorder;
		p2.y = p1.y;

		if (utils::Raycast(vertices, p1, p2, hitinfo) && velocity.x > 0)
		{
			velocity.x = 0;
			shape.left = hitinfo.intersectPoint.x - shape.width - outerBorder;
		}

		p2.x = p1.x - shape.width / 2 - outerBorder;

		if (utils::Raycast(vertices, p1, p2, hitinfo) && velocity.x < 0)
		{
			velocity.x = 0;
			shape.left = hitinfo.intersectPoint.x;
		}
	}
}

std::weak_ptr<Point2f> Pyramid::GetTarget()
{
	return m_pTarget;
}
