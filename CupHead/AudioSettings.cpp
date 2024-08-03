#include "pch.h"
#include "AudioSettings.h"
#include "Texture.h"
#include "MenuWord.h"
#include "ParticleSoundeffectManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

AudioSettings::AudioSettings(const Rectf& window)
	:m_Window{window}
{
	std::string textpath{ "./resources/images/UI/Menu/" };
	m_pCardTexture = new Texture{ textpath + "Big_Card.png" };

	textpath = "./resources/fonts/Vogue-Extra-Bold.otf";
	Color4f color1{ 0.2196078431372549f,0.2196078431372549f,0.24313725490196078f, 1.f };
	Color4f color2{ 0.7019607843137254f,0.1450980392156863f,0.16862745098039217f, 1.f };

	m_pWords.push_back(new MenuWord{ "Master Volume:", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Music Volume:", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "SFX Volume:", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Back", textpath, color1, color2 });
	
	m_pHyphen = new MenuWord{ "-", textpath, color1, color2 };
	m_pVerticalBar = new MenuWord{ "l", textpath, color1, color2 };
}

AudioSettings::~AudioSettings()
{
	delete m_pCardTexture;
	m_pCardTexture = nullptr;
	delete m_pHyphen;
	m_pHyphen = nullptr;
	delete m_pVerticalBar;
	m_pVerticalBar = nullptr;
	for (int i{}; i < m_pWords.size(); i++)
	{
		delete m_pWords[i];
		m_pWords[i] = nullptr;
	}
	m_pWords.clear();
}

void AudioSettings::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Window.width / 2 - m_pCardTexture->GetWidth() / 2, m_Window.height / 2 - m_pCardTexture->GetHeight() / 2, 0);

	m_pCardTexture->Draw();
	Point2f point;
	point.x = m_pCardTexture->GetWidth() / 2;
	point.y = m_pCardTexture->GetHeight() / 2;

	point.y += m_pWords[0]->GetHeight() * ((m_pWords.size()) / 2);

	int audioLevels[3];
	audioLevels[0] = g_pSFXManager->GetMasterVolume();
	audioLevels[1] = g_pSFXManager->GetMusicVolume();
	audioLevels[2] = g_pSFXManager->GetSFXVolume();
	bool selected{ false };

	for (int i{}; i < m_pWords.size(); i++)
	{
		if (m_Selected == i)selected = true;
		else selected = false;

		point.x = m_pCardTexture->GetWidth() / 2 - m_pWords[i]->GetWidth()/2 + 50;
		m_pWords[i]->Draw(point, selected);
		point.x += m_pWords[i]->GetWidth() / 2 + 10;

		if(i < 3) DrawBar(point, audioLevels[i], selected);

		point.y -= m_pWords[i]->GetHeight();
	}
	
	glPopMatrix();
}

void AudioSettings::Space()
{
	if (m_Selected == 3)
	{
		m_ShouldGoBack = true;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
	}
}

void AudioSettings::Escape()
{
	m_ShouldGoBack = true;
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
}

void AudioSettings::Up()
{
	--m_Selected %= int(m_pWords.size());
	if (m_Selected < 0)m_Selected = int(m_pWords.size()) - 1;
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
}

void AudioSettings::Right()
{
	switch (m_Selected)
	{
	case 0:
		g_pSFXManager->ChangeMasterVolume(true);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case 1:
		g_pSFXManager->ChangeMusicVolume(true);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case 2:
		g_pSFXManager->ChangeSFXVolume(true);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	default:
		break;
	}
}

void AudioSettings::Down()
{
	++m_Selected %= int(m_pWords.size());
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
}

void AudioSettings::Left()
{
	switch (m_Selected)
	{
	case 0:
		g_pSFXManager->ChangeMasterVolume(false);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case 1:
		g_pSFXManager->ChangeMusicVolume(false);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	case 2:
		g_pSFXManager->ChangeSFXVolume(false);
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		break;
	default:
		break;
	}
}

bool AudioSettings::ShouldGoBack()
{
	if (m_ShouldGoBack)
	{
		m_ShouldGoBack = false;
		m_Selected = 0;
		return true;
	}
	return false;
}

void AudioSettings::DrawBar(Point2f pos, int audioLevel, bool selected) const
{
	for (int i{}; i < audioLevel; i++)
	{
		m_pHyphen->Draw(pos, selected);
		pos.x += m_pHyphen->GetWidth();
	}

	m_pVerticalBar->Draw(pos, selected);
	pos.x += m_pHyphen->GetWidth();

	for (int i{}; i < 12 - audioLevel; i++)
	{
		m_pHyphen->Draw(pos, selected);
		pos.x += m_pHyphen->GetWidth();
	}
}
