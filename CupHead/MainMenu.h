#pragma once
class Texture;
class OscillatingSpriteSheet;
class ScreenEffectSpriteSheet;
class ScreenEffects;
class MenuWord;
class Settings;
#include <vector>
#include "Game.h"
#include "Enums.h"

class MainMenu final
{
public:
	MainMenu(const Rectf& window, Enums::Character cuphead, bool startUp = false);
	~MainMenu();

	MainMenu(MainMenu& rhs) = delete;
	MainMenu(MainMenu&& rhs) = delete;

	MainMenu& operator=(MainMenu& rhs) = delete;
	MainMenu& operator=(MainMenu&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec, ScreenEffects* screenEffects, Enums::Character& cupHead);

	void NextScreen();
	void PreviouseScreen();

	bool StartGame();

	void Reset();

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();

	enum class State
	{
		Black,
		Logo,
		TitleScreen,
		MainMenu,
		Settings
	};

private:
	Rectf m_Window;

	float m_BlackTimer{};
	const float m_BlackTime{ 2.f };

	ScreenEffectSpriteSheet* m_pLogoTexture;

	Texture* m_pTitleScreenBackground;
	OscillatingSpriteSheet* m_pTitleScreenAnimation;

	Texture* m_pMainMenuBackground;

	State m_MainMenuState{ State::TitleScreen };

	std::vector<MenuWord*> m_pWords;
	int m_Selected{ 0 };

	Settings* m_pSettings;

	float m_Timer{};
	const float m_TimerDuration{1};

	bool m_NextScreen{ false };
	bool m_StartGame{ false };

	void DrawMainMenu() const; 
	void DrawSettings() const;
};

