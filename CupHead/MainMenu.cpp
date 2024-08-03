#include "pch.h"
#include "MainMenu.h"
#include "Texture.h"
#include "OscillatingSpriteSheet.h"
#include "ScreenEffectSpriteSheet.h"
#include "ScreenEffects.h"
#include "MenuWord.h"
#include "Settings.h"
#include "ParticleSoundeffectManager.h"
#include "Game.h"
#include <string>

extern ParticleSoundeffectManager* g_pSFXManager;

MainMenu::MainMenu(const Rectf& window, Enums::Character cuphead, bool startUp)
	:m_Window{window}
{
	std::string textpath{ "./resources/images/UI/menu/" };

	m_pLogoTexture = new ScreenEffectSpriteSheet{ textpath + "Title_Screen_Logo.png", 5, 20, 100, false};

	m_pTitleScreenBackground = new Texture{ textpath + "title_screen_background.png" };
	m_pTitleScreenAnimation = new OscillatingSpriteSheet{ textpath + "title_screen_animation.png", 7, 5, 34 };

	m_pMainMenuBackground = new Texture{ textpath + "Menu_BackGround.png" };

	textpath = "./resources/fonts/Vogue-Extra-Bold.otf";
	Color4f color1{ 0.4196078431372549f,0.41568627450980394f,0.37254901960784315f, 1.f };
	Color4f color2{ 0.996078431372549f,1.0f,0.996078431372549f, 1.f };
	m_pWords.push_back(new MenuWord{ "Start", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Settings", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Exit", textpath, color1, color2 });

	m_pSettings = new Settings(window, cuphead);

	if (startUp)
	{
		m_MainMenuState = State::Black;
		g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::Logo, false);
	}
	else g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::Intro, false);
}

MainMenu::~MainMenu()
{
	delete m_pLogoTexture;
	m_pLogoTexture = nullptr;
	delete m_pTitleScreenBackground;
	m_pTitleScreenBackground = nullptr;
	delete m_pTitleScreenAnimation;
	m_pTitleScreenAnimation = nullptr;
	delete m_pMainMenuBackground;
	m_pMainMenuBackground = nullptr;
	for (int i{}; i < m_pWords.size(); i++)
	{
		delete m_pWords[i];
		m_pWords[i] = nullptr;
	}
	delete m_pSettings;
	m_pSettings = nullptr;
}

void MainMenu::Draw() const
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::Logo:
		m_pLogoTexture->Draw(m_Window);
		break;
	case MainMenu::State::TitleScreen:
		m_pTitleScreenBackground->Draw(m_Window);
		m_pTitleScreenAnimation->DrawBottom(Point2f{ m_Window.width / 2, 0 });
		break;
	case MainMenu::State::MainMenu:
		DrawMainMenu();
		break;
	case MainMenu::State::Settings:
		DrawSettings();
		break;
	default:
		break;
	}
}

void MainMenu::Update(float elapsedSec, ScreenEffects* screenEffects, Enums::Character& cupHead)
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::Black:
		m_BlackTimer += elapsedSec;
		if (m_BlackTimer >= m_BlackTime)
		{
			m_BlackTimer = 0;
			m_MainMenuState = State::Logo;
			screenEffects->Transition_In();
		}
		break;
	case MainMenu::State::Logo:
		m_pLogoTexture->Update(elapsedSec);
		if (m_pLogoTexture->GetFinishedCycle())
		{
			m_MainMenuState = State::TitleScreen;
			g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::Intro, false);
		}
		break;
	case MainMenu::State::TitleScreen:
		m_pTitleScreenAnimation->Update(elapsedSec);
		break;
	case MainMenu::State::Settings:
		m_pSettings->Update(elapsedSec, cupHead);
		if (m_pSettings->ShouldGoBack())PreviouseScreen();
	default:
		break;
	}


	if (m_NextScreen && !screenEffects->IsTransitioning() && !Game::GetCanTransition())
	{
		screenEffects->Transition_Out();
		m_NextScreen = false;
	}

	if (Game::GetCanTransition() && !m_StartGame)
	{
		m_Timer += elapsedSec;
		if (m_Timer >= m_TimerDuration)
		{
			NextScreen();
			screenEffects->TwoStates(ScreenEffects::State::Idle, ScreenEffects::State::Idle);
		}
	}
}

bool MainMenu::StartGame()
{
	return m_StartGame;
}

void MainMenu::Reset()
{
	m_MainMenuState = State::MainMenu;
	m_Selected = 0;
	m_StartGame = false;
	m_NextScreen = false;
	g_pSFXManager->PlaySong(ParticleSoundeffectManager::Music::Intro);
}

void MainMenu::NextScreen()
{
	m_MainMenuState = static_cast<State>(int(m_MainMenuState) + 1);
}

void MainMenu::PreviouseScreen()
{
	m_MainMenuState = static_cast<State>(int(m_MainMenuState) - 1);
	m_Selected = 0;
}

void MainMenu::Space()
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::TitleScreen:
		if(!m_NextScreen)m_NextScreen = true;
		break;
	case MainMenu::State::MainMenu:
		if (m_Selected == 0)m_StartGame = true;
		else if (m_Selected == 1)NextScreen();
		else Game::SetExitGame(true);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		break;
	case MainMenu::State::Settings:
		m_pSettings->Space();
		break;
	default:
		break;
	}
}

void MainMenu::Escape()
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::TitleScreen:
		break;
	case MainMenu::State::MainMenu:
		PreviouseScreen();
		break;
	case MainMenu::State::Settings:
		m_pSettings->Escape();
		break;
	default:
		break;
	}
}

void MainMenu::Up()
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::TitleScreen:
		break;
	case MainMenu::State::MainMenu:
		--m_Selected %= int(m_pWords.size());
		if (m_Selected < 0)m_Selected = int(m_pWords.size())-1;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case MainMenu::State::Settings:
		m_pSettings->Up();
		break;
	default:
		break;
	}
}

void MainMenu::Right()
{
	if (m_MainMenuState == State::Settings)m_pSettings->Right();
}

void MainMenu::Down()
{
	switch (m_MainMenuState)
	{
	case MainMenu::State::TitleScreen:
		break;
	case MainMenu::State::MainMenu:
		++m_Selected %= int(m_pWords.size());
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case MainMenu::State::Settings:
		m_pSettings->Down();
		break;
	default:
		break;
	}
}

void MainMenu::Left()
{
	if (m_MainMenuState == State::Settings)m_pSettings->Left();
}

void MainMenu::DrawMainMenu() const
{
	Point2f point;
	point.x = m_Window.width / 2;
	point.y = m_Window.height / 2;
	
	point.y += m_pWords[0]->GetHeight() * ((m_pWords.size()-2)/2);

	m_pMainMenuBackground->Draw(m_Window);

	for (int i{}; i < m_pWords.size(); i++)
	{
		if (m_Selected == i)m_pWords[i]->Draw(point, true);
		else m_pWords[i]->Draw(point, false);

		point.y -= m_pWords[i]->GetHeight();
	}
}

void MainMenu::DrawSettings() const
{
	m_pMainMenuBackground->Draw(m_Window);
	m_pSettings->Draw();
}