#include "pch.h"
#include "PlaneTutorial.h"
#include "SpriteSheet.h"
#include "LoopSpriteSheet.h"

PlaneTutorial::PlaneTutorial(const Rectf& window)
	:Level(window, "./resources/images/levels/planetutorial/Planetutorial.svg")
{
	m_pBackgroundTexture = new Texture{ "./resources/images/levels/planetutorial/BackGround.png" };

	m_pBackgroundAnimation = new LoopSpriteSheet{ "./resources/images/levels/planetutorial/BackGround_Animation.png",5, 9, 45, 40, 44 };

	m_pExitSign = new Texture{ "./resources/images/levels/planetutorial/Enter_Button.png" };
}

PlaneTutorial::~PlaneTutorial()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;

	delete m_pBackgroundAnimation;
	m_pBackgroundAnimation = nullptr;

	delete m_pExitSign;
	m_pExitSign = nullptr;
}

void PlaneTutorial::DrawLevel1() const
{
	m_pBackgroundTexture->Draw();
	if (m_PlayerInBox)m_pExitSign->Draw(m_ExitSignPos);
}

void PlaneTutorial::DrawLevel2() const
{
	m_pBackgroundAnimation->DrawBottom(Point2f{ m_pBackgroundAnimation->GetFrameWidth() / 2,0 });
}

void PlaneTutorial::UpdateLevel(float elapsedSec, const Rectf& player)
{
	m_pBackgroundAnimation->Update(elapsedSec);
	
	m_PlayerInBox = utils::IsOverlapping(player, m_ExitBox);
}

bool PlaneTutorial::ShouldExit()
{
	return m_PlayerInBox;
}

bool PlaneTutorial::IsInFrame(const Point2f& pos) const
{
	const float border{ 50.f };
	Rectf frame{ -border, -border, m_Window.width + 2 * border, m_Window.height + 2 * border };

	return utils::IsPointInRect(pos, frame);
}
