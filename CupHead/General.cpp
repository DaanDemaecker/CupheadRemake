#pragma once
#include "pch.h"
#include "General.h"
#include "ScreenEffects.h"
#include "ParticleSoundeffectManager.h"

#include "Level.h"
#include "Tutorial.h"
#include "PlaneTutorial.h"
#include "BotanicPanic.h"

#include "ObjectManager.h"
#include "ObjectManagerTutorial.h"
#include "ObjectManagerPlaneTutorial.h"
#include "ObjectManagerBotanicPanic.h"

#include "Avatar.h"
#include "NormalAvatar.h"
#include "ChaliceAvatar.h"
#include "PlaneAvatar.h"

extern ParticleSoundeffectManager* g_pSFXManager;

void General::InitializeLevel(Level*& level, Enums::Levels levelName, const Rectf& window)
{
	switch (levelName)
	{
	case Enums::Levels::Tutorial:
		level = new Tutorial{ window };
		break;
	case Enums::Levels::PlaneTutorial:
		level = new PlaneTutorial{ window };
		break;
	case Enums::Levels::BotanicPanic:
		level = new BotanicPanic{ window };
		break;
	default:
		break;
	}
}

void General::InitializeAvatar(Controller* controller, Avatar*& avatar, Enums::Levels levelName, PlayerEntry player, Level* level)
{
	switch (levelName)
	{
	case Enums::Levels::PlaneTutorial:
	//case Enums::Levels::BotanicPanic:
		avatar = new PlaneAvatar{controller, player.character, player.charm };
		break;
	case Enums::Levels::Tutorial:
	case Enums::Levels::BotanicPanic:
		if (player.charm != Enums::Charm::AstralCookie)avatar = new NormalAvatar{controller, player, level };
		else avatar = new ChaliceAvatar{controller, player, level };
		break;
	default:
		break;
	}
}

void General::InitializeObjectManager(ObjectManager*& objectManager, Enums::Levels levelName)
{
	switch (levelName)
	{
	case Enums::Levels::Tutorial:
		objectManager = new ObjectManagerTutorial();
		break;
	case Enums::Levels::PlaneTutorial:
		objectManager = new ObjectManagerPlaneTutorial();
		break;
	case Enums::Levels::BotanicPanic:
		objectManager = new ObjectManagerBotanicPanic();
		break;
	default:
		break;
	}
}

void General::LevelStart(Enums::Levels levelName, ScreenEffects* screenEffects)
{
	switch (levelName)
	{
	case Enums::Levels::Tutorial:
	case Enums::Levels::PlaneTutorial:
		g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::Tutorial);
		break;
	case Enums::Levels::BotanicPanic:
		g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::BotanicPanic);
		break;
	default:
		break;
	}

	if (levelName != Enums::Levels::Tutorial && levelName != Enums::Levels::PlaneTutorial)screenEffects->TwoStates(ScreenEffects::State::TransitionIn, ScreenEffects::State::Intro);
	else screenEffects->Transition_In();
}

Point2f General::GetOverworldPos(Enums::Levels level)
{
	switch (level)
	{
	case Enums::Levels::Tutorial:
		return Point2f{ 811, 1409 };
		break;
	case Enums::Levels::PlaneTutorial:
		return Point2f{ 2863, 1972 };
		break;
	case Enums::Levels::BotanicPanic:
		return Point2f{ 1988, 695 };
		break;
	default:
		return Point2f{ 811, 1409 };
		break;
	}
}
