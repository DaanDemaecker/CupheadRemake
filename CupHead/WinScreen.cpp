#include "pch.h"
#include "WinScreen.h"
#include "Texture.h"
#include "OscillatingSpriteSheet.h"
#include "LoopSpriteSheet.h"
#include "MenuWord.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"
#include <string>
#include <fstream>

extern ParticleSoundeffectManager* g_pSFXManager;

WinScreen::WinScreen(const Rectf& window, const PlayerEntry& player)
	:m_Window{window}
{
	ReadFromFile();

	std::string character;

	if (player.charm == Enums::Charm::AstralCookie)character = "Chalice";
	else
	{
		if (player.character == Enums::Character::CupHead)character = "Cuphead";
		else character = "Mugman";
	}

	std::string textPath{"./resources/images/ui/winscreen/"};
	m_pBackGroundTexture = new Texture{ textPath + "BackGround.png" };

	m_pTheResultTexture = new Texture{ textPath + "Title.png" };
	m_pTitleTexture = new Texture{ textPath + "Mdhr_Title.png" };

	m_pCharacterSpritesheet = new OscillatingSpriteSheet{ textPath + character + "_Animation.png", 4, 2, 7 };
	m_pCharacterTitle = new Texture{ textPath + character + "_Title.png" };
	
	m_pBoardTexture = new Texture{ textPath + "Board.png" };

	m_pStar1Texture = new LoopSpriteSheet{ textPath + "Star.png", 3, 3, 9, 8, 8};
	m_pStar2Texture = new LoopSpriteSheet{ textPath + "Star.png", 3, 3, 9, 8, 8};

	m_pCirlceTexture = new LoopSpriteSheet{ textPath + "Circle.png", 4, 3, 12, 11, 11};

	textPath = "./resources/fonts/Memphis-Medium.otf";
	Color4f color1{ 0.984313725490196f,0.9294117647058824f,0.8352941176470589f, 1.f };
	Color4f color2{ 0.996078431372549f,0.796078431372549f,0.24705882352941178f, 1.f };

	for (int i{}; i < 10; i++)m_pNumberTextures.push_back(new MenuWord{ std::to_string(i), textPath, color1, color2, 26});
	m_pColonTexture = new Texture{ ":", textPath, 26, color1 };


	textPath = "./resources/fonts/consola.ttf";
	m_pGradeTexture = new Texture{ GetGrade(), textPath, 34, color1};
	m_pSlashTexture = new Texture{ "/", textPath, 26, color1};

}

WinScreen::~WinScreen()
{
	delete m_pBackGroundTexture;
	m_pBackGroundTexture = nullptr;
	delete m_pTheResultTexture;
	m_pTheResultTexture = nullptr;
	delete m_pTitleTexture;
	m_pTitleTexture = nullptr;
	delete m_pCharacterSpritesheet;
	m_pCharacterSpritesheet = nullptr;
	delete m_pCharacterTitle;
	m_pCharacterTitle = nullptr;
	delete m_pBoardTexture;
	m_pBoardTexture = nullptr;
	delete m_pStar1Texture;
	m_pStar1Texture = nullptr;
	delete m_pStar2Texture;
	m_pStar2Texture = nullptr;
	delete m_pCirlceTexture;
	m_pCirlceTexture = nullptr;
	

	for (int i{}; i < m_pNumberTextures.size(); i++)
	{
		delete m_pNumberTextures[i];
		m_pNumberTextures[i] = nullptr;
	}
	m_pNumberTextures.clear();
	delete m_pColonTexture;
	m_pColonTexture = nullptr;
	delete m_pSlashTexture;
	m_pSlashTexture = nullptr;
	delete m_pGradeTexture;
	m_pGradeTexture = nullptr;
}

void WinScreen::ReadFromFile()
{
	std::ifstream file;
	file.open("./resources/TextFiles/MatchResult.txt");
	std::string line;

	if (file.is_open())
	{
		std::getline(file, line, '>');
		m_TargetTime = std::stoi(utils::GetAttributeValue("Time", line));
		std::getline(file, line, '>');
		m_HpBonus = std::stoi(utils::GetAttributeValue("HpBonus", line));
		std::getline(file, line, '>');
		m_Parry = std::stoi(utils::GetAttributeValue("ParryAmount", line));
		std::getline(file, line, '>');
		m_SuperMeter = std::stoi(utils::GetAttributeValue("SuperMeter", line));
		std::getline(file, line, '>');
	}

	file.close();
}

void WinScreen::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Window.width / 2, m_Window.height / 2, 0);
	glRotatef(m_Angle, 0, 0, 1);
	glTranslatef(-m_pBackGroundTexture->GetWidth() / 2, -m_pBackGroundTexture->GetHeight() / 2, 0);
	m_pBackGroundTexture->Draw();
	glPopMatrix();

	m_pTheResultTexture->Draw(Point2f{ m_Window.width / 2 - m_pTheResultTexture->GetWidth() / 2, m_Window.height - m_pTheResultTexture->GetHeight() - 10 });
	m_pTitleTexture->Draw(Point2f{ m_Window.width / 2 - m_pTitleTexture->GetWidth() / 2, m_Window.height - m_pTheResultTexture->GetHeight() - m_pTitleTexture->GetHeight() });
	
	glPushMatrix();
	glTranslatef(m_Window.width/2 - (m_pCharacterSpritesheet->GetFrameWidth() + m_pBoardTexture->GetWidth())/2
					, m_Window.height/2 - m_pBoardTexture->GetHeight()/1.5f, 0);
	DrawCharacterAndBoard();
	glPopMatrix();
}

void WinScreen::Update(float elapsedSec)
{
	m_Angle += elapsedSec * m_AngleSpeed;
	if (m_Angle >= 360) m_Angle = 0;

	m_pCharacterSpritesheet->Update(elapsedSec);

	if(m_SuperMeterDone)m_pStar1Texture->Update(elapsedSec);
	
	if (!m_Star1Done && m_pStar1Texture->GetCrFrame() == 8)
	{
		m_Star1Done = true;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Star);
	}

	if(m_Star1Done)m_pStar2Texture->Update(elapsedSec);
		
	if (!m_Star2Done && m_pStar2Texture->GetCrFrame() == 8)
	{
		m_Star2Done = true;
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Chalk);
	}

	if (m_Star2Done)m_pCirlceTexture->Update(elapsedSec);

	m_Timer += elapsedSec;

	if (!m_SuperMeterDone)	m_CounterTimer += elapsedSec;
	if (m_CounterTimer >= m_CounterTime)
	{
		m_CounterTimer -= m_CounterTime;
		IncreaseNumber();
	}
}

void WinScreen::DrawCharacterAndBoard() const
{
	Point2f pos{m_pCharacterSpritesheet->GetFrameWidth()/2 - m_pCharacterTitle->GetWidth()/2, 40};

	m_pCharacterTitle->Draw(pos);
	pos.x = m_pCharacterSpritesheet->GetFrameWidth() / 2;
	pos.y += m_pTitleTexture->GetHeight() * 4;

	m_pCharacterSpritesheet->DrawBottom(pos);

	pos.x = m_pCharacterSpritesheet->GetFrameWidth();
	pos.y = 30;

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	DrawBoard();
	glPopMatrix();

}

void WinScreen::DrawBoard() const
{
	const float rightBorder{ 440 };

	m_pBoardTexture->Draw();

	Point2f pos{rightBorder, 360};

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	//utils::FillEllipse(pos, 5, 5);

	bool selected{ false };
#pragma region time
	if (m_TimeDone && (m_TargetTime < m_MaxTime))selected = true;
	else selected = false;

	m_pNumberTextures[GetTime(m_DisplayTime, 0)]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[GetTime(m_DisplayTime, 0)]->GetWidth();

	m_pNumberTextures[GetTime(m_DisplayTime, 1)]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[GetTime(m_DisplayTime, 1)]->GetWidth();

	m_pColonTexture->Draw(pos);
	pos.x -= m_pColonTexture->GetWidth();

	m_pNumberTextures[GetTime(m_DisplayTime, 2)]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[GetTime(m_DisplayTime, 2)]->GetWidth();

	m_pNumberTextures[GetTime(m_DisplayTime, 3)]->Draw(pos, selected);

#pragma endregion time

#pragma region hpBonus
	if (m_HpBonusDone && (m_HpBonus == m_HpBonusMax))selected = true;
	else selected = false;

	pos.x = rightBorder;
	pos.y -= 45;

	m_pNumberTextures[m_HpBonusMax]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[m_HpBonusMax]->GetWidth()*1.4f;

	m_pSlashTexture->Draw(pos);
	pos.x -= m_pSlashTexture->GetWidth()/2;

	m_pNumberTextures[m_DisplayHpBonus]->Draw(pos, selected);

#pragma endregion hpBonus

#pragma region parry
	if (m_ParryDone && (m_Parry == m_ParryMax))selected = true;
	else selected = false;

	pos.x = rightBorder;
	pos.y -= 45;

	m_pNumberTextures[m_ParryMax]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[m_ParryMax]->GetWidth() * 1.4f;

	m_pSlashTexture->Draw(pos);
	pos.x -= m_pSlashTexture->GetWidth() / 2;

	m_pNumberTextures[m_DisplayParry]->Draw(pos, selected);

#pragma endregion parry

#pragma region supermeter
	if (m_SuperMeterDone && (m_SuperMeter == m_SuperMeterMax))selected = true;
	else selected = false;

	pos.x = rightBorder;
	pos.y -= 45;

	m_pNumberTextures[m_SuperMeterMax]->Draw(pos, selected);
	pos.x -= m_pNumberTextures[m_SuperMeterMax]->GetWidth() * 1.4f;

	m_pSlashTexture->Draw(pos);
	pos.x -= m_pSlashTexture->GetWidth() / 2;

	m_pNumberTextures[m_DisplaySuperMeter]->Draw(pos, selected);

#pragma endregion supermeter
	
#pragma region Star
	pos.x = rightBorder - m_pStar1Texture->GetFrameWidth()/2;
	pos.y -= 45 + m_pStar1Texture->GetFrameHeight()/4;

	if(m_SuperMeterDone)m_pStar1Texture->DrawBottom(pos);
	
	pos.x += m_pStar1Texture->GetFrameWidth() / 2;
	if(m_pStar1Texture->GetCrFrame() == 8) m_pStar2Texture->DrawBottom(pos);
#pragma endregion Star

#pragma region Grade
	if (m_pStar2Texture->GetCrFrame() == 8)
	{

		pos.x = rightBorder - 70;
		pos.y -= 45 + m_pGradeTexture->GetHeight() / 2;

		m_pGradeTexture->Draw(pos);

		pos.x += m_pCirlceTexture->GetFrameWidth() / 4;
		pos.y -= 5;
		m_pCirlceTexture->DrawBottom(pos);
	}
#pragma endregion Grade
}

int WinScreen::GetTime(int time, int position) const
{
	int result{};
	int seconds{time % 60};
	int minutes{time / 60};

	switch (position)
	{
	case 0:
		result = seconds - (int(seconds/10)* 10);
		break;
	case 1:
		result = int(seconds / 10);
		break;
	case 2:
		result = minutes - (int(minutes / 10) * 10);
		break;
	case 3:
		result = int(minutes / 10);
		break;
	default:
		break;
	}
	return result;
}

bool WinScreen::ShouldGoToMenu()
{
	if (m_Timer >= m_Time)return true;
	else return false;
}

std::string WinScreen::GetGrade()
{
	int minusPoints{};
	if (m_TargetTime > m_MaxTime)minusPoints++;
	minusPoints += m_HpBonusMax - m_HpBonus;
	minusPoints += m_ParryMax - m_Parry;
	minusPoints += m_SuperMeterMax - m_SuperMeter;

	switch (minusPoints)
	{
	case 0:
		return "A+";
		break;
	case 1:
		return "A";
		break;
	case 2:
		return "A-";
		break;
	case 3:
		return "B+";
		break;
	case 4:
		return "B";
		break;
	case 5:
		return "B-";
		break;
	case 6:
		return "C+";
		break;
	case 7:
		return "C";
		break;
	case 8:
		return "C-";
		break;
	case 9:
		return "D+";
		break;
	case 10:
		return "D";
		break;
	case 11:
		return "D-";
		break;
	default:
		return "F";
		break;
	}
}

void WinScreen::IncreaseNumber()
{
	if (!m_TimeDone)
	{
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Tick);
		if (++m_DisplayTime >= m_TargetTime)
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Timer_End);
			m_CounterTime = .3f;
			m_TimeDone = true;
		}
	}
	else if (!m_HpBonusDone)
	{
		if (m_HpBonus <= 0)m_HpBonusDone = true;
		else
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Tick);
			if (++m_DisplayHpBonus >= m_HpBonus)m_HpBonusDone = true;
		}

	}
	else if (!m_ParryDone)
	{

		if (m_Parry <= 0)m_ParryDone = true;
		else
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Tick);
			if (++m_DisplayParry >= m_Parry)m_ParryDone = true;
		}
	}
	else if (!m_SuperMeterDone)
	{
		if (m_SuperMeter <= 0)m_SuperMeterDone = true;
		else
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Tick);
			if (++m_DisplaySuperMeter >= m_SuperMeter)
			{
				g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Winscreen_Star);
				m_SuperMeterDone = true;
			}
		}
	}
}