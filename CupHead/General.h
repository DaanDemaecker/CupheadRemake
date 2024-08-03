#pragma once
#include <map>
#include "Enums.h"
class Level;
class ObjectManager;
class Avatar;
class ScreenEffects;

namespace General
{
#pragma region functions
	void InitializeLevel(Level*& level, Enums::Levels levelName, const Rectf& window);
	void InitializeAvatar(Controller* controller, Avatar*& avatar, Enums::Levels levelName, PlayerEntry player, Level* level);
	void InitializeObjectManager(ObjectManager*& objectManager, Enums::Levels levelName);
	void LevelStart(Enums::Levels levelName, ScreenEffects* screenEffects);

	Point2f GetOverworldPos(Enums::Levels level);
#pragma endregion functions
}