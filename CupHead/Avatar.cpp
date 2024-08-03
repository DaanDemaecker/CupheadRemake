#include "pch.h"
#include "Avatar.h"
#include "Level.h"
#include "utils.h"
#include "SpriteManager.h"
#include "ParticleSoundeffectManager.h"
#include "ObjectManager.h"
#include "Hud.h"
#include "Shadow.h"
#include "Controls.h"
#include <iostream>
#include <string>
#include <fstream>

Avatar::Avatar(Controller* controller)
	:m_pController{controller}
{
}

Avatar::~Avatar()
{
}

void Avatar::CharmSuperCharge(float elapsedSec, float& super, Enums::Charm charm)
{
	if (charm == Enums::Charm::Coffee || charm == Enums::Charm::DivineRelic)IncreaseSuper(elapsedSec * 0.04f);
	else if (charm == Enums::Charm::CursedRelic2)IncreaseSuper(elapsedSec * 0.013f);
	else if (charm == Enums::Charm::CursedRelic3)IncreaseSuper(elapsedSec * 0.026f);
	else if (charm == Enums::Charm::CursedRelic4)IncreaseSuper(elapsedSec * 0.039f);
}

bool Avatar::ShouldAxeParry(Enums::Charm charm, int parryAmount)
{
	if ((charm == Enums::Charm::Whetstone || charm == Enums::Charm::DivineRelic) ||
		(charm == Enums::Charm::CursedRelic1 && (parryAmount % 8) == 0) ||
		(charm == Enums::Charm::CursedRelic2 && (parryAmount % 5) == 0) ||
		(charm == Enums::Charm::CursedRelic3 && (parryAmount % 3) == 0) ||
		(charm == Enums::Charm::CursedRelic4 && (parryAmount % 2) == 0))
	{
		return true;
	}
	return false;
}

bool Avatar::RelicEquiped(Enums::Charm charm)
{
	if (charm == Enums::Charm::CursedRelic1 || charm == Enums::Charm::CursedRelic2 ||
		charm == Enums::Charm::CursedRelic3 || charm == Enums::Charm::CursedRelic4 ||
		charm == Enums::Charm::DivineRelic)return true;
	return false;
}

void Avatar::Input(bool checkKeyboard)
{
	const Uint8* pStates{ nullptr };

	if(checkKeyboard)pStates = SDL_GetKeyboardState(nullptr);

	bool buttonPressed{ false };
	for (auto& pair : m_CurrentInput)
	{
		buttonPressed = false;

		if (m_pController->active)
		{
			buttonPressed = m_pController->IsButtonPressed(pair.first);
		}

		if (checkKeyboard)
		{
			buttonPressed = buttonPressed || pStates[Controls::GetButton(pair.first)];
		}

		pair.second = buttonPressed;
	}
}
