#include "pch.h"
#include "OverworldSettings.h"
#include "Game.h"
#include "Texture.h"
#include "AudioSettings.h"
#include "MenuWord.h"
#include "utils.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

OverworldSettings::OverworldSettings(const Rectf& window)
	:m_Window{ window }
{
	m_pAudioSettings = new AudioSettings{ window };
	m_pCardTexture = new Texture{ "./resources/images/UI/Menu/Small_Card.png" };

	std::string textpath{ "./resources/fonts/Vogue-Extra-Bold.otf" };
	Color4f color1{ 0.2196078431372549f,0.2196078431372549f,0.24313725490196078f, 1.f };
	Color4f color2{ 0.7019607843137254f,0.1450980392156863f,0.16862745098039217f, 1.f };

	m_pWords.push_back(new MenuWord{ "Resume", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Audio Settings", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Exit To Title", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Quit Game", textpath, color1, color2 });
}

OverworldSettings::~OverworldSettings()
{
	delete m_pAudioSettings;
	m_pAudioSettings = nullptr;
	delete m_pCardTexture;
	m_pCardTexture = nullptr;
	for (int i{}; i < m_pWords.size(); i++)
	{
		delete m_pWords[i];
		m_pWords[i] = nullptr;
	}
	m_pWords.clear();
}

void OverworldSettings::Draw() const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
	utils::FillRect(m_Window);

	if (m_AudioSettings)m_pAudioSettings->Draw();
	else
	{
		glPushMatrix();
		glTranslatef(m_Window.width / 2 - m_pCardTexture->GetWidth() / 2, m_Window.height / 2 - m_pCardTexture->GetHeight() / 2, 0);

		m_pCardTexture->Draw();
		Point2f point;

		point.x = m_pCardTexture->GetWidth() / 2;
		point.y = m_pCardTexture->GetHeight() / 2;

		point.y += m_pWords[0]->GetHeight() * ((m_pWords.size()) / 3);

		bool selected{ false };

		for (int i{}; i < m_pWords.size(); i++)
		{
			if (m_Selected == i)selected = true;
			else selected = false;
			m_pWords[i]->Draw(point, selected);
			point.y -= m_pWords[i]->GetHeight();
		}

		glPopMatrix();
	}
}

void OverworldSettings::Update(bool& paused)
{
	if (m_AudioSettings)
	{
		if (m_pAudioSettings->ShouldGoBack())m_AudioSettings = false;
	}

	if (m_ShouldGoBack)
	{
		m_Selected = 0;
		m_ShouldGoBack = false;
		paused = false;
	}
}

void OverworldSettings::Space()
{
	if (m_AudioSettings)m_pAudioSettings->Space();
	else
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		switch (m_Selected)
		{
		case 0:
			m_ShouldGoBack = true;
			break;
		case 1:
			m_AudioSettings = true;
			break;
		case 2:
			m_Exit = true;
			break;
		case 3:
			Game::SetExitGame(true);
			break;
		default:
			break;
		}
	}
}

void OverworldSettings::Escape()
{
	if (m_AudioSettings)m_pAudioSettings->Escape();
	else
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		m_ShouldGoBack = true;
	}
}

void OverworldSettings::Up()
{
	if (m_AudioSettings)m_pAudioSettings->Up();
	else
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		--m_Selected %= int(m_pWords.size());
		if (m_Selected < 0)m_Selected = int(m_pWords.size()) - 1;
	}
}

void OverworldSettings::Right()
{
	if (m_AudioSettings)m_pAudioSettings->Right();
}

void OverworldSettings::Down()
{
	if (m_AudioSettings)m_pAudioSettings->Down();
	else
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		++m_Selected %= int(m_pWords.size());
	}
}

void OverworldSettings::Left()
{
	if (m_AudioSettings)m_pAudioSettings->Left();
}

bool OverworldSettings::ShouldExit()
{
	return m_Exit;
}

//bool OverworldSettings::ShouldGoBack()
//{
//	if (m_ShouldGoBack)
//	{
//		m_ShouldGoBack = false;
//		m_Selected = 0;
//		return true;
//	}
//	return false;
//}
