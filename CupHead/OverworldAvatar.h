#pragma once
class Texture;
#include "Vector2f.h"
#include "Enums.h"
#include <map>

class SpriteSheet;
class OverworldIsle;

class OverworldAvatar
{
public:
	OverworldAvatar(Controller* controller, Enums::Character character, Enums::Levels level, bool victory);
	~OverworldAvatar();

	OverworldAvatar(OverworldAvatar& rhs) = delete;
	OverworldAvatar(OverworldAvatar&& rhs) = delete;

	OverworldAvatar& operator=(OverworldAvatar& rhs) = delete;
	OverworldAvatar& operator=(OverworldAvatar&& rhs) = delete;

	void Draw();

	void Update(float elapsedSec, OverworldIsle* isle);

	Point2f GetPos();

	enum class State {
		Idle_Up,
		Idle_Side_Up,
		Idle_Side,
		Idle_Side_Down,
		Idle_Down,
		Walk_Up,
		Walk_Side_Up,
		Walk_Side,
		Walk_Side_Down,
		Walk_Down,
		Victory
	};

private:
	std::map<State, SpriteSheet*> m_pSpriteSheets;

	std::map<Enums::Actions, bool> m_Actions;

	Texture* m_pEnterButton;

	Controller* m_pController;

	Point2f m_Pos;
	const float m_Offset{ 20.f };

	bool m_FacingRight{true};

	State m_State;

	Vector2f m_Velocity;

	const float m_Speed{ 300.f };

	float m_VictoryTimer{};
	const float m_VictoryTime{ 2.f };

	bool m_IsNearBuilding{ false };

	void CalculateState();
};

