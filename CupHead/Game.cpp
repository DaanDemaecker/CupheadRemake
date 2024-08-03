#include "pch.h"
#include "Game.h"
#include <iostream>
#include "Avatar.h"
#include "NormalAvatar.h"
#include "Level.h"
#include "Tutorial.h"
#include "PlaneTutorial.h"
#include "ObjectManager.h"
#include "ObjectManagerTutorial.h"
#include "ObjectManagerBotanicPanic.h"
#include "utils.h"
#include "ScreenEffects.h"
#include "ParticleSoundeffectManager.h"
#include "MainMenu.h"
#include "PauseSettings.h"
#include "WinScreen.h"
#include "LoseScreen.h"
#include "General.h"
#include "OverworldAvatar.h"
#include "OverworldIsle.h"
#include "OverworldSettings.h"
#include "EquipmentCard.h"
#include "Controls.h"
#include <fstream>

ParticleSoundeffectManager* g_pSFXManager{nullptr};

bool Game::m_CanTransition{ false };

bool Game::m_ExitGame{ false };

void Game::SetCanTransition(bool canTransition)
{
	m_CanTransition = canTransition;
}

bool Game::GetCanTransition()
{
	return m_CanTransition;
}

void Game::SetExitGame(bool exitGame)
{
	m_ExitGame = exitGame;
}

Game::Game( const Window& window ) 
	:m_Window{ window }
{
	std::cout << "Welcome to Cuphead, by Daan Demaecker\n";
	std::cout << "Thank you for playing my version of the 2017 game by Studio MDHR\n";
	std::cout << "Use WASD or ZQSD to navigate trough menus \n";
	std::cout << "Press SPACE to select and ESCAPE to go back a page \n";
	std::cout << "Press I for more info or play the tutorial \n";
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	LoadFromFile();
	if(g_pSFXManager == nullptr)g_pSFXManager = new ParticleSoundeffectManager{};
	m_pScreenEffects = new ScreenEffects{ Rectf{0, 0, m_Window.width, m_Window.height} };
	InitializeMainMenu(true);
	g_pSFXManager->InitializeSFXMenu();
}

void Game::LoadFromFile()
{
	std::ifstream file;
	file.open("./resources/TextFiles/Data.txt");
	std::string line;

	int masterVolume, musicVolume, sfxVolume;

	Enums::Character character{};
	Enums::BulletType bulletType1{}, bulletType2{};
	Enums::Charm charm{};
	int cursedLevel{};

	if (file.is_open())
	{
		std::getline(file, line, '>');
		m_Level = static_cast<Enums::Levels>(std::stoi(utils::GetAttributeValue("Level", line)));
		std::getline(file, line, '>');
		character = static_cast<Enums::Character>(std::stoi(utils::GetAttributeValue("CupHead", line)));
		std::getline(file, line, '>');
		bulletType1 = static_cast<Enums::BulletType>(std::stoi(utils::GetAttributeValue("BulletType1", line)));
		std::getline(file, line, '>');
		bulletType2 = static_cast<Enums::BulletType>(std::stoi(utils::GetAttributeValue("BulletType2", line)));
		std::getline(file, line, '>');
		charm =  static_cast<Enums::Charm>(std::stoi(utils::GetAttributeValue("Charm", line)));
		std::getline(file, line, '>');
		cursedLevel = std::stoi(utils::GetAttributeValue("CursedLevel", line));
		std::getline(file, line, '>');

		masterVolume = std::stoi(utils::GetAttributeValue("MasterVolume", line));
		std::getline(file, line, '>');
		musicVolume = std::stoi(utils::GetAttributeValue("MusicVolume", line));
		std::getline(file, line, '>');
		sfxVolume = std::stoi(utils::GetAttributeValue("SFXVolume", line));
		g_pSFXManager = new ParticleSoundeffectManager{ masterVolume, musicVolume, sfxVolume };
	}

	file.close();

	m_PlayerEntry = PlayerEntry{ character, bulletType1, bulletType2, charm, cursedLevel };

	Controls::LoadControlsKeyboard();

	m_pController = new Controller();

	Controls::LoadControlsController();
}

void Game::Cleanup()
{
	SaveToFile();
	DeleteLevel();
	DeleteOverworld();
	DeleteMainMenu();
	if (m_pScreenEffects != nullptr) {
		delete m_pScreenEffects;
		m_pScreenEffects = nullptr;
	}
	if (g_pSFXManager != nullptr) {
		delete g_pSFXManager;
		g_pSFXManager = nullptr;
	}
	if (m_pWinScreen != nullptr)
	{
		delete m_pWinScreen;
		m_pWinScreen = nullptr;
	}
	if (m_pLoseScreen != nullptr)
	{
		delete m_pLoseScreen;
		m_pLoseScreen = nullptr;
	}

	delete m_pController;
	m_pController = nullptr;

}

void Game::SaveToFile()
{
	std::ofstream file;
	file.open("./resources/TextFiles/Data.txt");
	std::string line;

	if (file.is_open())
	{
		line = "<Level=\"" + std::to_string(int(m_Level)) + "\">\n";
		file << line;
		line = "<CupHead=\"" + std::to_string(int(m_PlayerEntry.character)) +"\">\n";
		file << line;
		line = "<BulletType1=\"" + std::to_string(int(m_PlayerEntry.bulletType1)) + "\">\n";
		file << line;
		line = "<BulletType2=\"" + std::to_string(int(m_PlayerEntry.bulletType2)) + "\">\n";
		file << line;
		line = "<Charm=\"" + std::to_string(int(m_PlayerEntry.charm)) + "\">\n";
		file << line;
		line = "<CursedLevel=\"" + std::to_string(int(m_PlayerEntry.cursedLevel)) + "\">\n";
		file << line;
		line = "<MasterVolume=\"" + std::to_string(g_pSFXManager->GetMasterVolume()) + "\">\n";
		file << line;
		line = "<MusicVolume=\"" + std::to_string(g_pSFXManager->GetMusicVolume()) + "\">\n";
		file << line;
		line = "<SFXVolume=\"" + std::to_string(g_pSFXManager->GetSFXVolume()) + "\">\n";
		file << line;
	}

	file.close();
}

bool Game::ShouldQuit()
{
	return m_ExitGame;
}

void Game::DeleteMainMenu()
{
	if (m_pMainMenu != nullptr)
	{
		delete m_pMainMenu;
		m_pMainMenu = nullptr;
	}
}

void Game::InitializeMainMenu(bool intro)
{
	m_pMainMenu = new MainMenu{ Rectf{0, 0, m_Window.width, m_Window.height}, m_PlayerEntry.character, intro };
}

void Game::Reset()
{
	ResetBools();

	DeleteLevel();

	InitializeLevel();
}

void Game::DeleteLevel()
{
	if (m_pPauseSettings != nullptr)
	{
		delete m_pPauseSettings;
		m_pPauseSettings = nullptr;
	}
	if (m_pLevel != nullptr) {
		delete m_pLevel;
		m_pLevel = nullptr;
	}
	if (m_pAvatar != nullptr) {
		delete m_pAvatar;
		m_pAvatar = nullptr;
	}
	if (m_pObjectManager != nullptr) {
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}
	if (m_pWinScreen != nullptr)
	{
		delete m_pWinScreen;
		m_pWinScreen = nullptr;
	}
	if (m_pLoseScreen != nullptr)
	{
		delete m_pLoseScreen;
		m_pLoseScreen = nullptr;
	}
	g_pSFXManager->LevelEnd(m_Level);
}

void Game::Update( float elapsedSec )
{
	switch (m_State)
	{
	case Game::State::Main_Menu:
		m_pMainMenu->Update(elapsedSec, m_pScreenEffects, m_PlayerEntry.character);
		break;
	case Game::State::EquipmentCard:
		m_pEquipCard->Update(elapsedSec, m_PlayerEntry);
		m_pIsland->Update(elapsedSec, m_pOverworldAvatar->GetPos());
		break;
	case Game::State::Overworld:
		m_pIsland->Update(elapsedSec, m_pOverworldAvatar->GetPos());
		if (!m_OverworldPaused)m_pOverworldAvatar->Update(elapsedSec, m_pIsland);
		else m_pOverworldSettings->Update(m_OverworldPaused);
		break;
	case Game::State::Playing:
		if (!m_Paused)
		{
			UpdateLevel(elapsedSec);
			ObjectCollisions();
			if (m_Knockout)HandleGameEnd(elapsedSec);
		}
		else m_pPauseSettings->Update(m_Paused);
		break;
	case Game::State::Win_Screen:
		m_pWinScreen->Update(elapsedSec);
		if (m_pWinScreen->ShouldGoToMenu() && !m_LoadOverworld)
		{
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
		}
		break;
	case Game::State::Lose_Screen:
		m_pLoseScreen->Update(elapsedSec);
		UpdateLevel(elapsedSec);
		break;
	}

	CheckGameState();
	if(!m_Paused)g_pSFXManager->Update(elapsedSec);

	m_pScreenEffects->Update(elapsedSec, m_Paused);
}

void Game::Draw() const
{
	ClearBackground( );
	switch (m_State)
	{
	case Game::State::Main_Menu:
		m_pMainMenu->Draw();
		break;
	case Game::State::EquipmentCard:
	case Game::State::Overworld:
		DrawOverworld();
		break;
	case Game::State::Playing:
	case Game::State::Lose_Screen:
		DrawLevel();
		break;
	default:
		break;
	}

	m_pScreenEffects->DrawFirstLayer();

	switch (m_State)
	{
	case Game::State::Playing:
		if (m_Paused)m_pPauseSettings->Draw();
		break;
	case Game::State::Win_Screen:
		m_pWinScreen->Draw();
		break;
	case Game::State::Lose_Screen:
		m_pLoseScreen->Draw();
		break;
	default:
		break;
	}

	m_pScreenEffects->DrawSecondLayer();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (m_State)
	{
	case Game::State::Main_Menu:
		MenuProcessKeyUpEvent(e);
		break;
	case Game::State::EquipmentCard:
		switch (e.keysym.sym)
		{
		case SDLK_ESCAPE:
			m_pEquipCard->Escape();
			if (m_pEquipCard->ShouldGoBack())m_State = State::Overworld;
			break;
		case SDLK_SPACE:
			m_pEquipCard->Space();
			break;
		case SDLK_BACKSPACE:
			m_pEquipCard->BackSpace();
		case SDLK_UP:
			m_pEquipCard->Up();
			break;
		case SDLK_RIGHT:
			m_pEquipCard->Right();
				break;
		case SDLK_DOWN:
			m_pEquipCard->Down();
				break;
		case SDLK_LEFT:
			m_pEquipCard->Left();
				break;
		default:
			break;
		}
		break;
	case Game::State::Overworld:
		if (!m_OverworldPaused)
		{
			switch (e.keysym.sym)
			{
			case SDLK_LSHIFT:
				m_State = State::EquipmentCard;
				break;
			case SDLK_RETURN:
				if (m_pIsland->IsAvatarNearBuilding())OverWorldEnter(m_pIsland->GetLatestBuilding());
				break;
			case SDLK_ESCAPE:
				m_OverworldPaused = true;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (e.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_pOverworldSettings->Escape();
				break;
			case SDLK_SPACE:
				m_pOverworldSettings->Space();
				if (m_pOverworldSettings->ShouldExit())
				{
					m_ExitToMenu = true;
					m_pScreenEffects->Transition_Out();
				}
				break;
			case SDLK_UP:
				m_pOverworldSettings->Up();
				break;
			case SDLK_RIGHT:
				m_pOverworldSettings->Right();
				break;
			case SDLK_DOWN:
				m_pOverworldSettings->Down();
				break;
			case SDLK_LEFT:
				m_pOverworldSettings->Left();
				break;
			default:
				break;
			}
		}
		break;
	case Game::State::Playing:
		switch (e.keysym.sym)
		{
		case SDLK_ESCAPE:
			if (!m_Paused)
			{
				m_Paused = true;
				g_pSFXManager->Pause();
			}
			else m_pPauseSettings->Escape();
			break;
		case SDLK_RETURN:
			if (m_Paused)m_pPauseSettings->Space();
			else
			{
				if (m_Level == Enums::Levels::Tutorial)
				{
					if (static_cast<Tutorial*>(m_pLevel)->ShouldExit())
					{
						m_LoadOverworld = true;
						m_pScreenEffects->Transition_Out();
					}
				}
				else if (m_Level == Enums::Levels::PlaneTutorial)
				{
					if (static_cast<PlaneTutorial*>(m_pLevel)->ShouldExit())
					{
						m_LoadOverworld = true;
						m_pScreenEffects->Transition_Out();
					}
				}
			}
			break;
			break;
		case SDLK_UP:
			if (m_Paused)m_pPauseSettings->Up();
			break;
		case SDLK_RIGHT:
			if (m_Paused)m_pPauseSettings->Right();
			break;
		case SDLK_DOWN:
			if (m_Paused)m_pPauseSettings->Down();
			break;
		case SDLK_LEFT:
			if (m_Paused)m_pPauseSettings->Left();
			break;
		}
		break;
	case Game::State::Win_Screen:
		switch (e.keysym.sym)
		{
		case SDLK_SPACE:
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
			break;
		}
		break;
	case Game::State::Lose_Screen:
		switch (e.keysym.sym)
		{
		case SDLK_ESCAPE:
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
			break;
		case SDLK_UP:
			m_pLoseScreen->Up();
			break;
		case SDLK_DOWN:
			m_pLoseScreen->Down();
			break;
		case SDLK_SPACE:
			m_pLoseScreen->Space();
			break;
		}
	default:
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_State == State::Playing)
	{
		switch (e.button)
		{
		case SDL_BUTTON_LEFT:
			m_pAvatar->SetShooting(true);
			break;
		case SDL_BUTTON_RIGHT:
			m_pAvatar->SpecialAtk();
			break;
		}
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	if (m_State == State::Playing)
	{
		switch (e.button)
		{
		case SDL_BUTTON_LEFT:
			m_pAvatar->SetShooting(false);
			break;
		}
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InitializeOverworld(bool victory)
{
	m_pIsland = new OverworldIsle(Rectf{ 0, 0, m_Window.width, m_Window.height });
	m_pOverworldAvatar = new OverworldAvatar{ m_pController, m_PlayerEntry.character, m_Level, victory };
	m_pEquipCard = new EquipmentCard{Rectf{0,0,m_Window.width, m_Window.height} ,m_PlayerEntry};
	m_pOverworldSettings = new OverworldSettings{ Rectf{0,0,m_Window.width, m_Window.height} };

	m_pScreenEffects->Transition_In();
	g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::InkwellIsleOne, true);
}

void Game::DeleteOverworld()
{
	if (m_pIsland != nullptr)
	{
		delete m_pIsland;
		m_pIsland = nullptr;
	}

	if (m_pOverworldAvatar != nullptr)
	{
		delete m_pOverworldAvatar;
		m_pOverworldAvatar = nullptr;
	}

	if (m_pEquipCard != nullptr)
	{
		delete m_pEquipCard;
		m_pEquipCard = nullptr;
	}

	if (m_pOverworldSettings != nullptr)
	{
		delete m_pOverworldSettings;
		m_pOverworldSettings = nullptr;
	}
}

void Game::DrawOverworld() const
{
	glPushMatrix();
	m_pIsland->TranslateCamera(m_pOverworldAvatar->GetPos());

	m_pIsland->DrawBase(m_pOverworldAvatar->GetPos());

	m_pOverworldAvatar->Draw();

	m_pIsland->DrawTop(m_pOverworldAvatar->GetPos());
	glPopMatrix();

	if (m_State == State::EquipmentCard)m_pEquipCard->Draw(m_PlayerEntry.character);
	if (m_OverworldPaused)m_pOverworldSettings->Draw();
}

void Game::InitializeLevel()
{
	m_pPauseSettings = new PauseSettings{ Rectf{0, 0,m_Window.width, m_Window.height} };
	General::InitializeLevel(m_pLevel, m_Level, Rectf{ 0, 0, m_Window.width, m_Window.height });
	General::InitializeAvatar(m_pController, m_pAvatar, m_Level, m_PlayerEntry , m_pLevel);
	General::InitializeObjectManager(m_pObjectManager, m_Level);
	g_pSFXManager->InitializeParticles(m_Level);
	g_pSFXManager->InitializeSFX(m_Level);
	General::LevelStart(m_Level, m_pScreenEffects);
}

void Game::UpdateLevel(float elapsedSec)
{
	m_pLevel->UpdateCamera(elapsedSec, m_pAvatar->GetShape(), m_pAvatar->GetVelocity().x);
	m_pObjectManager->FixParallax(m_pLevel->GetCameraDifference());
	m_pLevel->UpdateLevel(elapsedSec, m_pAvatar->GetShape());
	m_pAvatar->Update(elapsedSec, m_pLevel, m_pObjectManager);
	m_pObjectManager->Update(elapsedSec, m_pLevel, m_pAvatar->GetShape());
}

void Game::DrawLevel() const
{
	//BackGround
	m_pLevel->DrawLevel1();
	m_pObjectManager->DrawLevel1();

	m_pLevel->DrawLevel2();
	m_pObjectManager->DrawLevel2();

	//MiddleGround
	glPushMatrix();
	m_pLevel->TranslateCameraMiddleground();
	m_pLevel->DrawLevel3();
	m_pObjectManager->DrawLevel3();
	glPopMatrix();

	//ForeGround
	glPushMatrix();
	m_pLevel->TranslateCameraForeground();
	m_pLevel->DrawLevel4();
	m_pObjectManager->DrawLevel4();

	g_pSFXManager->Draw();
	m_pAvatar->Draw();
	glPopMatrix();

	//Overlays
	m_pLevel->DrawLevel5();
	m_pLevel->DrawLevel5();
	m_pAvatar->DrawHud();
}

void Game::HandleGameEnd(float elapsedSec)
{
	if (!m_GoToWinscreen)
	{
		m_EndGameTimer += elapsedSec;
		if (m_EndGameTimer >= m_EndGameTime)
		{
			m_GoToWinscreen = true;
			m_pScreenEffects->Transition_Out();
			m_EndGameTimer = 0;
		}
	}
	else
	{
		if (m_CanTransition)
		{
			m_pWinScreen = new WinScreen{ Rectf{0, 0, m_Window.width, m_Window.height}, m_PlayerEntry };
			g_pSFXManager->StopAllSFX();
			m_State = State::Win_Screen;
			m_pScreenEffects->Transition_In();
			g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::WinScreen);
		}
	}
}

void Game::CheckGameState()
{
	if (m_State == State::Main_Menu)
	{
		if (m_pMainMenu->StartGame()&& !m_LoadOverworld)
		{
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
		}

		if (m_CanTransition)
		{
			if (m_LoadOverworld)
			{
				m_LoadOverworld = false;
				m_State = State::Overworld;
				DeleteMainMenu();
				InitializeOverworld(false);
			}
		}
	}
	else if (m_State == State::Overworld)
	{
		if (m_LoadLevel && m_CanTransition)
		{
			ResetBools();
			DeleteOverworld();
			InitializeLevel();
			m_State = State::Playing;
		}

		if (m_ExitToMenu && m_CanTransition)
		{
			ResetBools();
			DeleteOverworld();
			m_pScreenEffects->Idle();
			InitializeMainMenu(false);
			m_State = State::Main_Menu;
		}
	}
	else if (m_State == State::Playing)
	{
		if (!m_HasDied && m_pAvatar->IsDead())
		{
			m_HasDied = true;
			m_pScreenEffects->YouDied();
		}
		if (m_HasDied && m_pScreenEffects->IsIdle())
		{
			m_pLoseScreen = new LoseScreen(Rectf{0, 0, m_Window.width, m_Window.height}, m_PlayerEntry.character, m_pObjectManager->GetLoseScreenData());
			m_State = State::Lose_Screen;
		}
		

		if (m_Level == Enums::Levels::BotanicPanic)
		{
			if (!m_Knockout && static_cast<ObjectManagerBotanicPanic*>(m_pObjectManager)->IsGameOver())
			{
				m_pAvatar->SaveToFile();
				m_Knockout = true;
				m_pScreenEffects->Knockout();
			}
		}

		if (m_pPauseSettings->ShouldReset())
		{
			m_Reset = true;
			m_pScreenEffects->Transition_Out();
		}

		if (m_Reset && m_CanTransition)
		{
			m_Reset = false;
			Reset();
			delete m_pLoseScreen;
			m_pLoseScreen = nullptr;
		}

		if (!m_LoadOverworld && m_pPauseSettings->ShouldExit())
		{
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
		}

		if (m_LoadOverworld && m_CanTransition)
		{
			g_pSFXManager->StopAllSFX();
			ResetBools();

			DeleteLevel();
			InitializeOverworld(false);
			m_State = State::Overworld;

			m_pScreenEffects->Transition_In();	
		}
	}
	else if (m_State == State::Win_Screen)
	{
		if (m_LoadOverworld && m_CanTransition)
		{
			ResetBools();
			DeleteLevel();
			
			InitializeOverworld(true);

			m_State = State::Overworld;

			delete m_pWinScreen;
			m_pWinScreen = nullptr;

			m_pScreenEffects->Transition_In();
		}
	}
	else if (m_State == State::Lose_Screen)
	{
		if (!m_Reset && m_pLoseScreen->ShouldReset())
		{
			m_Reset = true;
			m_pScreenEffects->Transition_Out();
		}
		else if (m_Reset && m_CanTransition)
		{
			g_pSFXManager->StopAllSFX();
			m_Reset = false;
			Reset();
			m_State = State::Playing;
		}
		else if (!m_LoadOverworld && m_pLoseScreen->ShouldExitToOverworld())
		{
			m_LoadOverworld = true;
			m_pScreenEffects->Transition_Out();
		}
		else if (m_LoadOverworld && m_CanTransition)
		{
			g_pSFXManager->StopAllSFX();
			ResetBools();
			DeleteLevel();

			InitializeOverworld(false);

			m_State = State::Overworld;

			delete m_pLoseScreen;
			m_pLoseScreen = nullptr;
			m_pScreenEffects->Transition_In();
		}
	}
}

void Game::ObjectCollisions()
{
	Rectf shape{ m_pAvatar->GetShape() };
	if (m_pAvatar->GetParry())
	{
		if (m_pObjectManager->CheckParry(shape, m_pAvatar->GetParryDamage()))
		{
			m_pAvatar->Parry();
			g_pSFXManager->CreateParticle(Point2f{ shape.left + shape.width / 2,shape.bottom + shape.height / 2 }, ParticleSoundeffectManager::ParticleType::Parry_Spark);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Player_Parry_Slap);
		}
	}

	if (m_pAvatar->CanGetHit())
	{
		if (m_pObjectManager->Checkhits(shape, m_pAvatar->GetParry(), m_pAvatar->GetVelocity()))m_pAvatar->Hit();
	}

	m_pAvatar->IncreaseSuper(m_pObjectManager->CheckBullets(m_pAvatar->GetBullets(), m_pAvatar->GetBigBullets())/150.f);
}

void Game::PrintInfo()
{
	std::cout << "Cuphead is a shooter/platformer \n";
	std::cout << "You can use A or Q to move left and D to move right \n";
	std::cout << "Hold S to duck \n";
	std::cout << "Press space to jump\n";
	std::cout << "Press space while in midair to parry pink objects \n";
	std::cout << "Press Shift to dash \n";
	std::cout << "Hold left mouse button to shoot \n";
	std::cout << "Hold lCtrl to stay in place while shooting, use WASD or ZQSD to aim \n";
	std::cout << "Press right click to use a special attack if you have at least 1 full playcard in the bottom left\n \n";
	std::cout << "Credit to RavenClawNimbus for most of the assets \n \n";
}

void Game::MenuProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintInfo();
		break;
	case SDLK_SPACE:
	case SDLK_RETURN:
		m_pMainMenu->Space();
		break;
	case SDLK_ESCAPE:
		m_pMainMenu->Escape();
		break;
	case SDLK_UP:
		m_pMainMenu->Up();
		break;
	case SDLK_RIGHT:
		m_pMainMenu->Right();
		break;
	case SDLK_DOWN:
		m_pMainMenu->Down();
		break;
	case SDLK_LEFT:
		m_pMainMenu->Left();
		break;
	}
}

void Game::OverWorldEnter(Enums::Buildings building)
{
	bool loadLevel{ true };

	switch (building)
	{
	case Enums::Buildings::Tutorial:
		m_Level = Enums::Levels::Tutorial;
		break;
	case Enums::Buildings::ShmupTutorial:
		m_Level = Enums::Levels::PlaneTutorial;
		break;
	case Enums::Buildings::BotanicPanic:
		m_Level = Enums::Levels::BotanicPanic;
		break;
	default:
		loadLevel = false;
		break;
	}

	if (loadLevel)
	{
		m_LoadLevel = true;
		m_pScreenEffects->Transition_Out();
	}

}

void Game::ResetBools()
{
	m_HasDied = false;
	m_Knockout = false;
	m_SpaceBarPressed = false;
	m_LoadLevel = false;
	m_Reset = false;
	m_Paused = false;
	m_LoadOverworld = false;
	m_GoToWinscreen = false;
	m_ExitToMenu = false;
	m_OverworldPaused = false;
}
