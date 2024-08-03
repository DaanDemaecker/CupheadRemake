#include "pch.h"
#include "structs.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "Controls.h"
#include <iostream>

//-----------------------------------------------------------------
// Window Constructors
//-----------------------------------------------------------------
Window::Window( const std::string& title , float width , float height , bool isVSyncOn )
	:title{ title }
	,width{ width }
	,height{ height }
	,isVSyncOn{ isVSyncOn }
{
}

//-----------------------------------------------------------------
// Point2f Constructors
//-----------------------------------------------------------------
Point2f::Point2f( )
	:Point2f{ 0.0f, 0.0f }
{
}
Point2f::Point2f( float x, float y )
	:x{ x }, y{ y }
{
}

bool operator!=(const Point2f& lhs, const Point2f& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)return true;
	else return false;
}

//-----------------------------------------------------------------
// Point3f Constructors
//-----------------------------------------------------------------
Point3f::Point3f()
	:Point3f{ 0.0f, 0.0f, 0.f }
{
}
Point3f::Point3f(float x, float y, float z)
	: x{ x }, y{ y }, z{z}
{
}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf( )
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf( float left, float bottom, float width, float height )
	:left{ left }
	,bottom{ bottom }
	,width{ width }
	,height{ height }
{
}

//-----------------------------------------------------------------
// Color4f Constructors
//-----------------------------------------------------------------
Color4f::Color4f( )
	:Color4f{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Color4f::Color4f( float r, float g, float b, float a )
	:r{ r }
	,g{ g }
	,b{ b }
	,a{ a }
{
}

//-----------------------------------------------------------------
// Circlef Constructors
//-----------------------------------------------------------------
Circlef::Circlef( )
	:Circlef{ 0.0f, 0.0f, 0.0f }
{
}

Circlef::Circlef( float centerX, float centerY, float radius )
	:Circlef{ Point2f{ centerX, centerY }, radius }
{
}

Circlef::Circlef( const Point2f& center, float radius )
	:center{ center }
	,radius{ radius }
{
}

//-----------------------------------------------------------------
// Ellipsef Constructors
//-----------------------------------------------------------------
Ellipsef::Ellipsef( )
	:Ellipsef{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}


Ellipsef::Ellipsef( const Point2f& center, float radiusX, float radiusY )
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef( float centerX, float centerY, float radiusX, float radiusY )
	: Ellipsef{ Point2f{ centerX, centerY }, radiusX, radiusY }
{
}


//-----------------------------------------------------------------
// Controller Constructors
//-----------------------------------------------------------------
Controller::Controller()
{
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::cerr << "GameController::GameController( ), error when calling SDL_INIT: " << SDL_GetError() << std::endl;
	}
	bool found{ false };
	for (int i = 0; i < SDL_NumJoysticks() && !found; i++)
	{

		if (SDL_IsGameController(i))
		{
			controller = SDL_GameControllerOpen(i);
			if (controller)
			{
				found = true;
				joystick = SDL_GameControllerGetJoystick(controller);
				active = true;
			}
		}
	}
	if (!found)
	{
		std::cerr << "No controller found!\n";
	}
}


//-----------------------------------------------------------------
// ControllerButtonEntry Constructors
//-----------------------------------------------------------------
ControllerButtonEntry::ControllerButtonEntry()
	:isButton{ true }
{
	axis = SDL_CONTROLLER_AXIS_INVALID;
	button = SDL_CONTROLLER_BUTTON_INVALID;
}

ControllerButtonEntry::ControllerButtonEntry(SDL_GameControllerAxis axis)
	:axis{axis}, isButton{false}
{
	button = SDL_CONTROLLER_BUTTON_INVALID;
}

ControllerButtonEntry::ControllerButtonEntry(SDL_GameControllerButton button)
	:button{button}, isButton{true}
{
	axis = SDL_CONTROLLER_AXIS_INVALID;
}

bool Controller::IsButtonPressed(Enums::Actions action)
{
	bool buttonPressed{ false };

	if (Controls::GetControllerButton(action).isButton)
	{
		buttonPressed = SDL_GameControllerGetButton(controller, Controls::GetControllerButton(action).button);
	}
	else
	{
		buttonPressed = SDL_GameControllerGetAxis(controller, Controls::GetControllerButton(action).axis);
	}

	switch (action)
	{
	case Enums::Actions::Up:
		buttonPressed = buttonPressed || (SDL_JoystickGetAxis(joystick, 1) < -tolerance);
		break;
	case Enums::Actions::Down:
		buttonPressed = buttonPressed || (SDL_JoystickGetAxis(joystick, 1) > tolerance);
		break;
	case Enums::Actions::Left:
		buttonPressed = buttonPressed || (SDL_JoystickGetAxis(joystick, 0) < -tolerance);
		break;
	case Enums::Actions::Right:
		buttonPressed = buttonPressed || (SDL_JoystickGetAxis(joystick, 0) > tolerance);
		break;
	default:
		break;
	}

	return buttonPressed;
}
			

//-----------------------------------------------------------------
// PlayerEntry Constructors
//-----------------------------------------------------------------
PlayerEntry::PlayerEntry()
	:character{Enums::Character::CupHead}, bulletType1{Enums::BulletType::PeaShooter}, bulletType2{Enums::BulletType::none},
	charm{Enums::Charm::None}, cursedLevel{}
{

}


PlayerEntry::PlayerEntry(Enums::Character character, Enums::BulletType bulletType1, Enums::BulletType bulletType2,
		Enums::Charm charm, int cursedLevel)
	:character{ character }, bulletType1{ bulletType1 }, bulletType2{ bulletType2 }, charm{ charm }, cursedLevel{ cursedLevel }
{

}


//-----------------------------------------------------------------
// LoseScreenData Constructors
//-----------------------------------------------------------------
LoseScreenData::LoseScreenData()
	:LoseScreenData{true}
{
}

LoseScreenData::LoseScreenData(float distance, float* nodes, int nodeAmount, const std::string& textPath)
	:distance{distance}, nodes{nodes}, nodeAmount{nodeAmount}, textPath{textPath}, secret{false}
{
}

LoseScreenData::LoseScreenData(bool secret, const std::string& textPath)
	:secret{secret}, nodes{nullptr}, nodeAmount{}, textPath{textPath}, distance{}
{
}

//-----------------------------------------------------------------
// ParticleEntry Constructors
//-----------------------------------------------------------------
ParticleEntry::ParticleEntry(Enums::EffectSubDivision subdivision, std::string textPath, int cols, int rows, int frames, float frameSpeed)
	:subdivision{subdivision}, nrCols{cols}, nrRows{rows}, nrFrames{frames}, frameSpeed{frameSpeed}
{
	pSpriteSheet = new Texture{ textPath };
}

ParticleEntry::~ParticleEntry()
{
	delete pSpriteSheet;
	pSpriteSheet = nullptr;
}


//-----------------------------------------------------------------
// SFXEntry Constructors
//-----------------------------------------------------------------
SFXEntry::SFXEntry(Enums::EffectSubDivision subdivision, std::string textPath)
	:subdivision{subdivision}
{
	pSoundEffect = new SoundEffect{ textPath };
}

SFXEntry::~SFXEntry()
{
	delete pSoundEffect;
	pSoundEffect = nullptr;
}


//-----------------------------------------------------------------
// OverworldBuilding Constructors
//-----------------------------------------------------------------
OverworldBuilding::OverworldBuilding(const std::string& textPath, int frames, const Point2f& pos, float height)
	:pos{ pos }, height{ height }
{
	pSpritesheet = new SpriteSheet{ textPath, frames, 1, frames, 1 / 15.f };
	hitBox = Ellipsef{ Point2f{pos.x, pos.y + pSpritesheet->GetFrameHeight() / 2},
						pSpritesheet->GetFrameWidth() / 1.7f, pSpritesheet->GetFrameHeight() / 1.7f };
}

OverworldBuilding::~OverworldBuilding()
{
	delete pSpritesheet;
	pSpritesheet = nullptr;
}

//-----------------------------------------------------------------
// OverworldObstacle Constructors
//-----------------------------------------------------------------
OverworldObstacle::OverworldObstacle(bool unLocked, const std::string& textPath, const Point2f& pos)
	:unLocked{ unLocked }, pos{ pos }
{
	pTexture = new Texture{ textPath };
}

OverworldObstacle::~OverworldObstacle()
{
	delete pTexture;
	pTexture = nullptr;
}


