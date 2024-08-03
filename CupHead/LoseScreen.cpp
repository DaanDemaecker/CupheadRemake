#include "pch.h"
#include "LoseScreen.h"
#include "Texture.h"
#include "SpriteSheet.h"
#include "MenuWord.h"
#include "utils.h"
#include "Game.h"
#include <iostream>

LoseScreen::LoseScreen(const Rectf& window, Enums::Character cuphead, LoseScreenData loseScreenData)
	:m_Window{window}, m_Distance{loseScreenData.distance}, m_pNodes{loseScreenData.nodes}, m_NodeAmount{loseScreenData.nodeAmount}
	, m_Secret{loseScreenData.secret}
{
	std::string textpath{ "./resources/images/ui/losescreen/" };

	m_pCardTexture = new Texture{textpath + "Card.png"};
	m_pMugShotTexture = new Texture{ loseScreenData.textPath };
	if(!m_Secret)m_pBarTexture = new Texture{ textpath + "Bar.png" };
	else m_pBarTexture = new Texture{ textpath + "Secret_Bar.png" };
	m_pNodeTexture = new Texture{ textpath + "Node.png" };

	if (cuphead == Enums::Character::CupHead)m_pPlayerTexture = new SpriteSheet{ textpath + "CupHead_Running.png", 4, 2, 8 };
	else m_pPlayerTexture = new SpriteSheet{ textpath + "MugMan_Running.png", 4, 2, 8 };

	textpath = "./resources/fonts/Vogue-Extra-Bold.otf";
	Color4f color1{ 0.2196078431372549f,0.2196078431372549f,0.24313725490196078f, 1.f };
	Color4f color2{ 0.7019607843137254f,0.1450980392156863f,0.16862745098039217f, 1.f };

	m_pWords.push_back(new MenuWord{ "Retry", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Exit To Map", textpath, color1, color2 });
	m_pWords.push_back(new MenuWord{ "Exit Game", textpath, color1, color2 });
}

LoseScreen::~LoseScreen()
{
	delete m_pCardTexture;
	m_pCardTexture = nullptr;
	delete m_pMugShotTexture;
	m_pMugShotTexture = nullptr;
	delete m_pBarTexture;
	m_pBarTexture = nullptr;
	delete m_pNodeTexture;
	m_pNodeTexture = nullptr;
	delete m_pPlayerTexture;
	m_pPlayerTexture = nullptr;
	delete[] m_pNodes;
	m_pNodes = nullptr;

	for (int i{}; i < m_pWords.size(); i++)
	{
		delete m_pWords[i];
		m_pWords[i] = nullptr;
	}
}

void LoseScreen::Draw() const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
	utils::FillRect(m_Window);

	glPushMatrix();
	glTranslatef(m_Window.width / 2 - m_pCardTexture->GetWidth() / 2, m_Window.height/2 - m_pCardTexture->GetHeight() / 2 - 15, 0);
	glRotatef(3, 0, 0, 1);
	DrawCard();
	glPopMatrix();
}

void LoseScreen::Update(float elapsedSec)
{
	if (!m_Secret)
	{
		m_pPlayerTexture->Update(elapsedSec);

		if (m_DisplayDistance < m_Distance)
		{
			m_DisplayDistance += (1.3f - (m_DisplayDistance / m_Distance)) * elapsedSec;
		}
	}
}

void LoseScreen::DrawCard() const
{
	m_pCardTexture->Draw();

	m_pMugShotTexture->Draw(Point2f{ m_pCardTexture->GetWidth() / 2 - m_pMugShotTexture->GetWidth() / 2, m_pCardTexture->GetHeight() - m_pMugShotTexture->GetHeight() - 50 });

	Point2f pos{m_pCardTexture->GetWidth()/2, 45 + 3 * m_pWords[1]->GetHeight()};
	bool selected{ false };
	for (int i{}; i < m_pWords.size(); i++)
	{
		if (m_Selected == i)selected = true;
		else selected = false;
		m_pWords[i]->Draw(pos, selected);
		pos.y -= m_pWords[i]->GetHeight();
	}


	glPushMatrix();
	glTranslatef(m_pCardTexture->GetWidth() / 2 - m_pBarTexture->GetWidth() / 2, m_pCardTexture->GetHeight() / 2 - 80, 0);
	DrawBar();
	glPopMatrix();
}

void LoseScreen::DrawBar() const
{
	float offSet{ 10 }, barWidth{m_pBarTexture->GetWidth()*0.93f};

	m_pBarTexture->Draw();

	if (!m_Secret)
	{
		for (int i{}; i < m_NodeAmount; i++)
		{
			m_pNodeTexture->Draw(Point2f{ offSet + m_pNodes[i] * barWidth, 11 });
		}

		m_pPlayerTexture->DrawBottom(Point2f{ offSet + barWidth * GetPlayerPos(),11 });
	}
}

float LoseScreen::GetPlayerPos() const
{

	float pos{};
	int lastNode{ int(m_DisplayDistance) - 1 };
	float lastNodePos{};
	float nextNodePos{1};

	if (lastNode > -1)lastNodePos = m_pNodes[lastNode];
	if (lastNode + 1 < m_NodeAmount)nextNodePos = m_pNodes[lastNode + 1];

	pos = lastNodePos;

	if (lastNode >= m_NodeAmount - 1)pos += (1 - lastNodePos) * (m_DisplayDistance - int(m_DisplayDistance));
	else pos += (nextNodePos - lastNodePos) * (m_DisplayDistance - int(m_DisplayDistance));

	return pos;
}

void LoseScreen::Up()
{
	if (--m_Selected < 0)m_Selected = int(m_pWords.size()) - 1;
}

void LoseScreen::Down()
{
	++m_Selected %= m_pWords.size();
}

void LoseScreen::Space()
{
	switch (m_Selected)
	{
	case 0:
		m_Reset = true;
		break;
	case 1:
		m_ExitMenu = true;
		break;
	case 2:
		Game::SetExitGame(true);
		break;
	}
}

bool LoseScreen::ShouldReset()
{
	return m_Reset;
}

bool LoseScreen::ShouldExitToOverworld()
{
	return m_ExitMenu;
}