#include "pch.h"
#include "OverworldAvatar.h"
#include "utils.h"
#include <string>
#include "SpriteSheet.h"
#include "OscillatingSpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "OverworldIsle.h"
#include "General.h"
#include "Controls.h"

#include <iostream>

OverworldAvatar::OverworldAvatar(Controller* controller, Enums::Character character, Enums::Levels level, bool victory)
	:m_pController{controller}
{
	m_Actions[Enums::Actions::Up] = false;
	m_Actions[Enums::Actions::Down] = false;
	m_Actions[Enums::Actions::Left] = false;
	m_Actions[Enums::Actions::Right] = false;
	
	m_Pos = General::GetOverworldPos(level);

	if (!victory)m_State = State::Idle_Down;
	else m_State = State::Victory;

	std::string textPath{ "./resources/images/overworld/avatar/" };
	
	if (character == Enums::Character::CupHead) textPath += "CupHead/"; 
	else textPath += "MugMan/";

	m_pSpriteSheets[State::Idle_Up] = new OscillatingSpriteSheet{textPath + "Idle_Up.png", 5, 1, 5};
	m_pSpriteSheets[State::Idle_Side_Up] = new OscillatingSpriteSheet{ textPath + "Idle_Side_Up.png", 5, 1, 5 };
	m_pSpriteSheets[State::Idle_Side] = new OscillatingSpriteSheet{ textPath + "Idle_Side.png", 5, 1, 5 };
	m_pSpriteSheets[State::Idle_Side_Down] = new OscillatingSpriteSheet{ textPath + "Idle_Side_Down.png", 5, 1, 5 };
	m_pSpriteSheets[State::Idle_Down] = new OscillatingSpriteSheet{ textPath + "Idle_Down.png", 5, 1, 5 };
	m_pSpriteSheets[State::Walk_Up] = new SpriteSheet{textPath + "Walk_Up.png", 16, 1, 16};
	m_pSpriteSheets[State::Walk_Side_Up] = new SpriteSheet{ textPath + "Walk_Side_Up.png", 16, 1, 16 };
	m_pSpriteSheets[State::Walk_Side] = new SpriteSheet{ textPath + "Walk_Side.png", 16, 1, 16};
	m_pSpriteSheets[State::Walk_Side_Down] = new SpriteSheet{ textPath + "Walk_Side_Down.png", 16, 1, 16};
	m_pSpriteSheets[State::Walk_Down] = new SpriteSheet{ textPath + "Walk_Down.png", 16, 1, 16};
	m_pSpriteSheets[State::Victory] = new LoopSpriteSheet{textPath + "Victory.png", 12, 1, 12, 0, 8};

	m_pEnterButton = new Texture{ "./resources/images/overworld/Enter_Button.png" };
}

OverworldAvatar::~OverworldAvatar()
{
	for (std::pair<State, SpriteSheet*> pair : m_pSpriteSheets)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSpriteSheets.clear();

	delete m_pEnterButton;
	m_pEnterButton = nullptr;
}

void OverworldAvatar::Draw()
{
	m_pSpriteSheets[m_State]->DrawBottom(Point2f{m_Pos.x, m_Pos.y - m_Offset}, m_FacingRight);

	if (m_IsNearBuilding)
	{
		m_pEnterButton->Draw(Point2f{ m_Pos.x - m_pEnterButton->GetWidth() / 2, m_Pos.y + m_pSpriteSheets[State::Idle_Down]->GetFrameHeight() / 1.3f });
	}
}

void OverworldAvatar::Update(float elapsedSec, OverworldIsle* isle)
{
	CalculateState();

	m_IsNearBuilding = isle->IsAvatarNearBuilding();

	switch (m_State)
	{
	case OverworldAvatar::State::Walk_Up:
		m_Velocity = Vector2f{ 0, m_Speed };
		break;
	case OverworldAvatar::State::Walk_Side_Up:
		m_Velocity = Vector2f{ .7f * m_Speed * (int(m_FacingRight) * 2 - 1), .7f * m_Speed };
		break;
	case OverworldAvatar::State::Walk_Side:
		m_Velocity = Vector2f{ m_Speed * (int(m_FacingRight) * 2 - 1), 0 };
		break;
	case OverworldAvatar::State::Walk_Side_Down:
		m_Velocity = Vector2f{ .7f * m_Speed * (int(m_FacingRight) * 2 - 1), -.7f * m_Speed };
		break;
	case OverworldAvatar::State::Walk_Down:
		m_Velocity = Vector2f{ 0, -m_Speed };
		break;
	case OverworldAvatar::State::Victory:
		if (m_VictoryTimer < m_VictoryTime)
		{
			m_VictoryTimer += elapsedSec;
			if (m_VictoryTimer > m_VictoryTime) static_cast<LoopSpriteSheet*>(m_pSpriteSheets[State::Victory])->StopLoop();
		}
		else if (m_pSpriteSheets[m_State]->GetCrFrame() >= 7)m_State = State::Idle_Down;
		break;
	default:
		m_Velocity = Vector2f{};
		break;
	}

	isle->HandleCollisions(m_Pos, m_Velocity);

	m_Pos += m_Velocity * elapsedSec;

	m_pSpriteSheets[m_State]->Update(elapsedSec);
}

Point2f OverworldAvatar::GetPos()
{
	return m_Pos;
}

void OverworldAvatar::CalculateState()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	for (auto& pair : m_Actions)
	{
		pair.second = pStates[Controls::GetButton(pair.first)];
	}

	if (m_pController->active)
	{
		for (auto& pair : m_Actions)
		{
			pair.second = pair.second || m_pController->IsButtonPressed(pair.first);

		}
	}


	bool up{ false }, down{ false }, left{ false }, right{ false };

	up = m_Actions[Enums::Actions::Up];
	down = m_Actions[Enums::Actions::Down];
	left = m_Actions[Enums::Actions::Left];
	right = m_Actions[Enums::Actions::Right];

	if (up || right || down || left)
	{
		if (up && !(left || right))m_State = State::Walk_Up;
		else if (up && (left || right))m_State = State::Walk_Side_Up;
		else if (!up && !down)m_State = State::Walk_Side;
		else if (down && (left || right))m_State = State::Walk_Side_Down;
		else if (down && !(left || right))m_State = State::Walk_Down;

		if (left) m_FacingRight = false;
		else if (right) m_FacingRight = true;

	}
	else
	{
		switch (m_State)
		{
		case OverworldAvatar::State::Walk_Up:
			m_State = State::Idle_Up;
			break;
		case OverworldAvatar::State::Walk_Side_Up:
			m_State = State::Idle_Side_Up;
			break;
		case OverworldAvatar::State::Walk_Side:
			m_State = State::Idle_Side;
			break;
		case OverworldAvatar::State::Walk_Side_Down:
			m_State = State::Idle_Side_Down;
			break;
		case OverworldAvatar::State::Walk_Down:
			m_State = State::Idle_Down;
			break;
		default:
			break;
		}
	}

}
