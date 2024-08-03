#pragma once
#include "Level.h"
#include "Enums.h"
class Avatar;
class ObjectManager;
class ScreenEffects;
class ParticleSoundeffectManager;
class MainMenu;
class EquipmentCard;
class PauseSettings;
class WinScreen;
class LoseScreen;
class OverworldAvatar;
class OverworldIsle;
class OverworldSettings;

#include "Vector2f.h"

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	bool ShouldQuit();

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

	static void SetCanTransition(bool canTransition);
	static bool GetCanTransition();

	static void SetExitGame(bool exitGame);
private:
	// DATA MEMBERS
	const Window m_Window;

	Controller* m_pController;

	static bool m_CanTransition;
	static bool m_ExitGame;

	ScreenEffects* m_pScreenEffects{ nullptr };

	Level* m_pLevel{nullptr};
	Avatar* m_pAvatar{ nullptr };
	ObjectManager* m_pObjectManager{ nullptr };

	OverworldIsle* m_pIsland{ nullptr };
	OverworldAvatar* m_pOverworldAvatar{ nullptr };
	OverworldSettings* m_pOverworldSettings{ nullptr };
	EquipmentCard* m_pEquipCard{ nullptr };

	PauseSettings* m_pPauseSettings{ nullptr };
	MainMenu* m_pMainMenu{ nullptr };

	WinScreen* m_pWinScreen{ nullptr };
	LoseScreen* m_pLoseScreen{ nullptr };

	bool m_SpaceBarPressed{ false };

	bool m_HasDied{ false };
	bool m_Knockout{ false };
	bool m_GoToWinscreen{false};
	bool m_Paused{ false };
	
	bool m_LoadOverworld{ false };
	bool m_OverworldPaused{ false };
	bool m_ShowEquipCard{ false };

	bool m_LoadLevel{ false };
	bool m_Reset{ false };

	bool m_ExitToMenu{ false };
	

	
	Enums::Levels m_Level;

	PlayerEntry m_PlayerEntry;

	float m_EndGameTimer{};
	const float m_EndGameTime{ 2.5f };

	enum class State
	{
		Main_Menu,
		Overworld,
		EquipmentCard,
		Playing,
		Win_Screen,
		Lose_Screen
	};
	State m_State{ State::Main_Menu };

	// FUNCTIONS
	void Initialize( );
	void LoadFromFile();
	void SaveToFile();

	void Cleanup( );
	void ClearBackground( ) const;

	void DeleteMainMenu();
	void InitializeMainMenu(bool intro);

	void Reset();
	void InitializeLevel();
	void DeleteLevel();

	void InitializeOverworld(bool victory);
	void DeleteOverworld();

	void DrawOverworld() const;

	void UpdateLevel(float elapsedSec);

	void DrawLevel() const;
	void HandleGameEnd(float elapsedSec);

	void ObjectCollisions();

	void CheckGameState();

	void PrintInfo();

	void MenuProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void OverWorldEnter(Enums::Buildings building);
	void ResetBools();
};