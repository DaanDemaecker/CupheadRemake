#include "pch.h"
#include "ScreenEffects.h"
#include "Texture.h"
#include <string>
#include "utils.h"
#include "ParticleSoundeffectManager.h"
#include "Game.h"

extern ParticleSoundeffectManager* g_pSFXManager;

ScreenEffects::ScreenEffects(const Rectf& window)
	:m_Window{ window }
{
	m_pStaticTexture = new ScreenEffectSpriteSheet{ "./resources/images/Screeneffects/static.png", 5, 7, 31, true, m_FrameSpeed };

	m_pTextures[State::TransitionIn] = new ScreenEffectSpriteSheet("./resources/images/Screeneffects/Transition.png", 3, 6, 17, false, m_FrameSpeed / 2);
	m_pTextures[State::TransitionOut] = new ScreenEffectSpriteSheet("./resources/images/Screeneffects/Transition.png", 3, 6, 17, false, m_FrameSpeed / 2);
	m_pTextures[State::Intro] = new ScreenEffectSpriteSheet("./resources/images/Screeneffects/Intro.png", 7, 7, 49, false, m_FrameSpeed);
	m_pTextures[State::YouDied] = new ScreenEffectSpriteSheet("./resources/images/Screeneffects/YouDied.png", 3, 7, 20, false, m_FrameSpeed);
	m_pTextures[State::Knockout] = new ScreenEffectSpriteSheet("./resources/images/Screeneffects/Knockout.png", 7, 4, 28, false, m_FrameSpeed);

	m_YouDiedRect.width = 2*window.width/3;
	m_YouDiedRect.height = window.height/3;
	m_YouDiedRect.left = window.width / 2 - m_YouDiedRect.width / 2;
	m_YouDiedRect.bottom = window.height / 2 - m_YouDiedRect.height / 2;
}

ScreenEffects::~ScreenEffects()
{
	delete m_pStaticTexture;
	m_pStaticTexture = nullptr;

	for (std::pair<State, ScreenEffectSpriteSheet*> pair : m_pTextures)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pTextures.clear();
}

void ScreenEffects::DrawFirstLayer()
{
	switch (m_State)
	{
	case ScreenEffects::State::Intro:
	case ScreenEffects::State::Knockout:
		m_pTextures[m_State]->Draw(m_Window);
		break;
	case ScreenEffects::State::YouDied:
		m_pTextures[m_State]->Draw(m_YouDiedRect);
		break;
	default:
		break;
	}
}

void ScreenEffects::DrawSecondLayer()
{
	switch (m_State)
	{
	case ScreenEffects::State::TransitionIn:
	case ScreenEffects::State::TransitionOut:
		m_pTextures[m_State]->Draw(m_Window);
		break;
	case ScreenEffects::State::Black:
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
		utils::FillRect(m_Window);
		break;
	default:
		break;
	}

	m_pStaticTexture->Draw(m_Window);
}

void ScreenEffects::Update(float elapsedsec, bool paused)
{
	m_pStaticTexture->Update(elapsedsec);

	if (!paused)
	{
		CheckState();
		if (m_State != State::Idle && m_State != State::Black)
		{
			m_pTextures[m_State]->Update(elapsedsec);
		}
	}
	else
	{
		CheckState();
		if ((m_State == State::TransitionIn || m_State == State::TransitionOut) &&
			(m_State != State::Idle && m_State != State::Black))
		{
			m_pTextures[m_State]->Update(elapsedsec);
		}
	}
	Game::SetCanTransition(m_State == State::Black);
}

void ScreenEffects::CheckState()
{
	if (m_State != State::Idle && m_State != State::Black)
	{
		if (m_pTextures[m_State]->GetFinishedCycle())
		{
			switch (m_NextState)
			{
			case ScreenEffects::State::TransitionIn:
				Transition_In();
				break;
			case ScreenEffects::State::TransitionOut:
				Transition_Out();
				break;
			case ScreenEffects::State::Intro:
				Intro();
				break;
			case ScreenEffects::State::YouDied:
				YouDied();
				break;
			case ScreenEffects::State::Knockout:
				Knockout();
				break;
			case ScreenEffects::State::Idle:
				m_State = State::Idle;
				break;
			case ScreenEffects::State::Black:
				m_State = State::Black;
				break;
			default:
				break;
			}
			if(m_NextState != State::Black)m_NextState = State::Idle;
		}
	}
}

void ScreenEffects::Intro()
{
	m_State = State::Intro;
	m_pTextures[m_State]->ResetCycle();
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Announcer_Intro);
}

void ScreenEffects::YouDied()
{
	m_State = State::YouDied;
	m_pTextures[m_State]->ResetCycle();
}

void ScreenEffects::Knockout()
{
	m_State = State::Knockout;
	m_pTextures[m_State]->ResetCycle();
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Announcer_Knockout);
}

void ScreenEffects::Idle()
{
	m_State = State::Idle;
	m_NextState = State::Idle;
}

void ScreenEffects::Transition_Out()
{
	m_State = State::TransitionOut;
	m_NextState = State::Black;
	m_pTextures[m_State]->ResetCycle(false);
}

void ScreenEffects::Transition_In()
{
	m_State = State::TransitionIn;
	m_NextState = State::Idle;
	m_pTextures[m_State]->ResetCycle();
}

bool ScreenEffects::IsIdle()
{
	if (m_State == State::Idle)return true;
	else return false;
}

bool ScreenEffects::IsTransitioning()
{
	if (m_State == State::TransitionIn || m_State == State::TransitionOut)return true;
	else return false;
}

void ScreenEffects::TwoStates(State firstState, State Secondstate)
{
	m_State = firstState;
	switch (m_State)
	{
	case ScreenEffects::State::TransitionIn:
		Transition_In();
		break;
	case ScreenEffects::State::TransitionOut:
		Transition_Out();
		break;
	case ScreenEffects::State::Intro:
		Intro();
		break;
	case ScreenEffects::State::YouDied:
		YouDied();
		break;
	case ScreenEffects::State::Knockout:
		Knockout();
		break;
	case ScreenEffects::State::Idle:
		m_State = State::Idle;
		break;
	case ScreenEffects::State::Black:
		m_State = State::Black;
		break;
	default:
		break;
	}
	m_NextState = Secondstate;
}