#include "pch.h"
#include "Carrot.h"
#include "utils.h"
#include "Matrix2x3.h"
#include "Texture.h"
#include "ParticleSoundeffectManager.h"
extern ParticleSoundeffectManager* g_pSFXManager;

Carrot::Carrot()
	:m_NrCols{3}, m_NrRows{2}, m_NrFrames{6}
{
	m_pCenter = std::make_shared<Point2f>(Point2f{});
	m_pTexture = new Texture{ "./resources/Images/Levels/BotanicPanic/Chauncey_Chantenay/Carrot_Torpedo.png" };
	m_pIntroTexture = new Texture{ "./resources/images/Levels/BotanicPanic/Chauncey_Chantenay/Carrot_Torpedo_Intro.png" };

	m_Width = m_pTexture->GetWidth()/m_NrCols;
	m_Height = m_pTexture->GetHeight()/m_NrRows;

	
	m_IntroPos = Point2f{float(rand() % 1280), 0};
	m_Angle = 3.f * float(M_PI) / 2.f;
	m_Speed = 200;
	m_RotSpeed = float(M_PI);
	
	m_OriginalShape.push_back(Point2f{-m_Width/2, -m_Height/2});
	m_OriginalShape.push_back(Point2f{ m_Width / 2, -m_Height / 2 });
	m_OriginalShape.push_back(Point2f{ m_Width / 2, m_Height / 2 });
	m_OriginalShape.push_back(Point2f{ -m_Width / 2, m_Height / 2 });
}


Carrot::~Carrot()
{
	delete m_pTexture;
	m_pTexture = nullptr;
	delete m_pIntroTexture;
	m_pIntroTexture = nullptr;
}

void Carrot::DrawIntro() const
{
	
	if (m_Intro)
	{
		Rectf srcRect{};
		srcRect.width = m_pIntroTexture->GetWidth() / m_NrCols;
		srcRect.height = m_pIntroTexture->GetHeight() / m_NrRows;
		srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
		srcRect.bottom = (m_CrFrame / m_NrCols) * srcRect.height + srcRect.height;

		m_pIntroTexture->Draw(m_IntroPos, srcRect);
	}
}

void Carrot::Draw() const
{
	if (!m_Intro && m_Activated)
	{
		Rectf srcRect{};
		srcRect.width = m_pTexture->GetWidth() / m_NrCols;
		srcRect.height = m_pTexture->GetHeight() / m_NrRows;
		srcRect.left = (m_CrFrame % m_NrCols) * srcRect.width;
		srcRect.bottom = (m_CrFrame / m_NrCols) * srcRect.height + srcRect.height;

		glPushMatrix();
		glTranslatef(m_pCenter->x, m_pCenter->y, 0);
		glRotatef(float(m_Angle / (2 * M_PI) * 360), 0, 0, 1);
		glTranslatef(-m_Width / 2, -m_Height / 2, 0);
		m_pTexture->Draw(Point2f{}, srcRect);

		glPopMatrix();

	}
}

void Carrot::Update(float elapsedSec, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets)
{
	UpdateTimer(elapsedSec);
	if (m_Intro)
	{
		m_IntroPos.y += m_Speed * elapsedSec;

		if (m_IntroPos.y >= 750)
		{
			m_Intro = false;
			*m_pCenter = m_IntroPos;
			targets.push_back(m_pCenter);
		}
	}
	else if (!m_Intro && m_Activated)
	{
		utils::RotateObjectToTarget(*m_pCenter, m_Angle, Point2f{shape.left + shape.width / 2, shape.bottom + shape.height / 2}, m_RotSpeed * elapsedSec);

		m_pCenter->x += m_Speed * float(cos(m_Angle)) * elapsedSec;
		m_pCenter->y += m_Speed * float(sin(m_Angle)) * elapsedSec;

		Matrix2x3 matTranslate, matRotate;
		matTranslate.SetAsTranslate(m_pCenter->x, m_pCenter->y);
		matRotate.SetAsRotate(float(m_Angle / (2 * M_PI) * 360));
		Matrix2x3 matWorld = matTranslate * matRotate;
		m_HitBox = matWorld.Transform(m_OriginalShape);
	}
}

void Carrot::FixAngle(float& angle)
{
	if (angle < 0)angle += float(M_PI) * 2;
	else if(angle >= 2*float(M_PI))angle -= float(M_PI) * 2;
}

bool Carrot::CheckHit(const Rectf& shape)
{
	if (m_Activated && utils::IsOverlapping(m_HitBox, shape))
	{
		m_Activated = false;
		g_pSFXManager->CreateParticle(*m_pCenter, ParticleSoundeffectManager::ParticleType::Carrot_Torpedo_Death);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Carrot_Death);
		return true;
		
	}
	else return false;
}

bool Carrot::CheckBulletHit(const Circlef& hitBox)
{
	if (m_Activated)
	{
		if (utils::IsOverlapping(m_HitBox, hitBox))
		{
			m_Activated = false;
			g_pSFXManager->CreateParticle(*m_pCenter, ParticleSoundeffectManager::ParticleType::Carrot_Torpedo_Death);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Chauncey_Chantenay_Carrot_Death);
			return true;
		}
	}

	return false;
}

bool Carrot::GetActivated()
{
	return m_Activated;
}

void Carrot::UpdateTimer(float elapsedSec)
{
	m_Timer += elapsedSec;
	
	if (m_Timer >= m_FrameSpeed)
	{
		m_Timer -= m_FrameSpeed;
		++m_CrFrame %= m_NrFrames;
	}
}