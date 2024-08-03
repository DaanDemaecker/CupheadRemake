#pragma once
#include "Enums.h"
#include "SoundEffect.h"
#include <string>
class Texture;
class SpriteSheet;

struct Window
{
	explicit Window( const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = true );

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	float x;
	float y;
};

struct Point3f
{
	Point3f();
	explicit Point3f(float x, float y, float z);

	float x;
	float y;
	float z;
};

bool operator!=(const Point2f& lhs, const Point2f& rhs);

struct Rectf
{
	Rectf( );
	explicit Rectf( float left, float bottom, float width, float height );
	
	float left;
	float bottom;
	float width;
	float height;

};


struct Color4f
{
	Color4f( );
	explicit Color4f( float r, float g, float b, float a );
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Point2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Point2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Point2f center;
	float radiusX;
	float radiusY;
};


struct Controller
{
	Controller();

	bool IsButtonPressed(Enums::Actions action);

	SDL_GameController* controller{nullptr};
	SDL_Joystick* joystick{ nullptr };
	bool active{ false };
	float tolerance{ INT16_MAX / 10 };
};


struct ControllerButtonEntry
{
	ControllerButtonEntry();
	ControllerButtonEntry(SDL_GameControllerAxis axis);
	ControllerButtonEntry(SDL_GameControllerButton button);

	SDL_GameControllerAxis axis;
	SDL_GameControllerButton button;
	bool isButton;
};


struct PlayerEntry
{
	PlayerEntry();
	explicit PlayerEntry(Enums::Character character, Enums::BulletType bulletType1, Enums::BulletType bulletType2,
		Enums::Charm charm, int cursedLevel);

	Enums::Character character;
	Enums::BulletType bulletType1;
	Enums::BulletType bulletType2;
	Enums::Charm charm;
	int cursedLevel;
};



struct LoseScreenData
{
	LoseScreenData();
	explicit LoseScreenData(float distance, float* nodes, int nodeAmount, const std::string& textPath);
	explicit LoseScreenData(bool secret, const std::string& textPath = std::string{});

	bool secret;
	float distance;
	float* nodes;
	int nodeAmount;
	std::string textPath;

};

struct ParticleEntry
{
	explicit ParticleEntry(Enums::EffectSubDivision subdivision, std::string textPath, int cols, int rows, int frames, float frameSpeed = 1 / 20.f);

	~ParticleEntry();

	Enums::EffectSubDivision subdivision;
	Texture* pSpriteSheet;
	int nrCols;
	int nrRows;
	int nrFrames;
	float frameSpeed;
};

struct SFXEntry
{
	explicit SFXEntry(Enums::EffectSubDivision subdivision, std::string textPath);
	
	~SFXEntry();

	Enums::EffectSubDivision subdivision;
	SoundEffect* pSoundEffect;

};

struct OverworldBuilding
{
	explicit OverworldBuilding(const std::string& textPath, int frames, const Point2f& pos, float height);
	~OverworldBuilding();

	SpriteSheet* pSpritesheet;
	Ellipsef hitBox;
	const Point2f pos;
	float height;
};

struct OverworldObstacle
{
	explicit OverworldObstacle(bool unLocked, const std::string& textPath, const Point2f& pos);
	~OverworldObstacle();

	bool unLocked;
	Texture* pTexture;
	const Point2f pos;
};