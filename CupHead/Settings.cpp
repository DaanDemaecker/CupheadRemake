#include "pch.h"
#include "Settings.h"
#include "Texture.h"
#include "MenuWord.h"
#include "AudioSettings.h"
#include "ParticleSoundeffectManager.h"
#include <iostream>

extern ParticleSoundeffectManager* g_pSFXManager;

Settings::Settings(const Rectf& window, Enums::Character cuphead)
	:m_Window{window}, m_Cuphead{cuphead}
{
	std::string textpath{ "./resources/images/UI/menu/" };
	m_pCardTexture = new Texture{ textpath + "Small_Card.png" };

	m_pAudioSettings = new AudioSettings{window};

	textpath = "./resources/fonts/Vogue-Extra-Bold.otf";
	Color4f color1{ 0.2196078431372549f,0.2196078431372549f,0.24313725490196078f, 1.f };
	Color4f color2{ 0.7019607843137254f,0.1450980392156863f,0.16862745098039217f, 1.f };
	
	m_pSettings1Words.push_back(new MenuWord{ "Audio Settings", textpath, color1, color2 });
	m_pSettings1Words.push_back(new MenuWord{ "Change Character", textpath, color1, color2 });
	m_pSettings1Words.push_back(new MenuWord{ "Back", textpath, color1, color2 });

	m_pSettings2Words.push_back(new MenuWord{ "CupHead", textpath, color1, color2 });
	m_pSettings2Words.push_back(new MenuWord{ "MugMan", textpath, color1, color2 });
	m_pSettings2Words.push_back(new MenuWord{ "Back", textpath, color1, color2 });
}

Settings::~Settings()
{
	delete m_pAudioSettings;
	m_pAudioSettings = nullptr;
	delete m_pCardTexture;
	m_pCardTexture = nullptr;
	for (int i{}; i < m_pSettings1Words.size(); i++)
	{
		delete m_pSettings1Words[i];
		m_pSettings1Words[i] = nullptr;
	}

	for (int i{}; i < m_pSettings2Words.size(); i++)
	{
		delete m_pSettings2Words[i];
		m_pSettings2Words[i] = nullptr;
	}
}

void Settings::Draw() const
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		DrawMainSettings();
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Draw();
		break;
	case Settings::State::Change_Character:
		DrawChangeCharacter();
		break;
	default:
		break;
	}
}

void Settings::Update(float elapsedSec, Enums::Character& cuphead)
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		break;
	case Settings::State::Audio_Settings:
		if (m_pAudioSettings->ShouldGoBack())
		{
			m_State = State::Main_Settings;
		}
		break;
	case Settings::State::Change_Character:
		break;
	default:
		break;
	}

	cuphead = m_Cuphead;
}

void Settings::Space()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		MainSettingsSpace();
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Space();
		break;
	case Settings::State::Change_Character:
		if (m_Selected2 == 0)m_Cuphead = Enums::Character::CupHead;
		else if (m_Selected2 == 1)m_Cuphead = Enums::Character::MugMan;
		m_State = State::Main_Settings;
		m_Selected2 = 0;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		break;
	default:
		break;
	}
}

void Settings::Escape()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		m_GoBack = true;
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Escape();
		break;
	case Settings::State::Change_Character:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		m_State = State::Main_Settings;
		break;
	default:
		break;
	}
}

void Settings::Up()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		--m_Selected1 %= int(m_pSettings1Words.size());
		if (m_Selected1 < 0)m_Selected1 = int(m_pSettings1Words.size()) - 1;
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Up();
		break;
	case Settings::State::Change_Character:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		--m_Selected2 %= int(m_pSettings2Words.size());
		if (m_Selected2 < 0)m_Selected2 = int(m_pSettings2Words.size()) - 1;
		break;
	default:
		break;
	}
}

void Settings::Right()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Right();
		break;
	case Settings::State::Change_Character:
		break;
	default:
		break;
	}
}

void Settings::Down()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		++m_Selected1 %= int(m_pSettings1Words.size());
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Down();
		break;
	case Settings::State::Change_Character:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		++m_Selected2 %= int(m_pSettings2Words.size());
		break;
	default:
		break;
	}
}

void Settings::Left()
{
	switch (m_State)
	{
	case Settings::State::Main_Settings:
		break;
	case Settings::State::Audio_Settings:
		m_pAudioSettings->Left();
		break;
	case Settings::State::Change_Character:
		break;
	default:
		break;
	}
}

bool Settings::ShouldGoBack()
{
	if (m_GoBack)
	{
		m_Selected1 = 0;
		m_GoBack = false;
		return true;
	}
	else return false;
}

void Settings::MainSettingsSpace()
{
	switch (m_Selected1)
	{
	case 0:
		m_State = State::Audio_Settings;
		break;
	case 1:
		m_State = State::Change_Character;
		break;
	case 2:
		m_GoBack = true;
		break;
	default:
		break;
	}
}

void Settings::DrawMainSettings() const
{
	glPushMatrix();
	glTranslatef(m_Window.width/2 - m_pCardTexture->GetWidth()/2, m_Window.height/2 - m_pCardTexture->GetHeight()/2, 0);
	

	m_pCardTexture->Draw();

	Point2f point;
	point.x = m_pCardTexture->GetWidth() / 2;
	point.y = m_pCardTexture->GetHeight() / 2;

	point.y += m_pSettings1Words[0]->GetHeight() * ((m_pSettings1Words.size()) / 2);


	for (int i{}; i < m_pSettings1Words.size(); i++)
	{
		if (m_Selected1 == i)m_pSettings1Words[i]->Draw(point, true);
		else m_pSettings1Words[i]->Draw(point, false);

		point.y -= m_pSettings1Words[i]->GetHeight();
	}

	glPopMatrix();
}

void Settings::DrawChangeCharacter() const
{
	glPushMatrix();
	glTranslatef(m_Window.width / 2 - m_pCardTexture->GetWidth() / 2, m_Window.height / 2 - m_pCardTexture->GetHeight() / 2, 0);


	m_pCardTexture->Draw();

	Point2f point;
	point.x = m_pCardTexture->GetWidth() / 2;
	point.y = m_pCardTexture->GetHeight() / 2;

	point.y += m_pSettings2Words[0]->GetHeight() * ((m_pSettings2Words.size()) / 2);


	for (int i{}; i < m_pSettings2Words.size(); i++)
	{
		if (m_Selected2 == i)m_pSettings2Words[i]->Draw(point, true);
		else m_pSettings2Words[i]->Draw(point, false);

		point.y -= m_pSettings2Words[i]->GetHeight();
	}
	glPopMatrix();
}