#include "pch.h"
#include "MenuWord.h"
#include "Texture.h"

MenuWord::MenuWord(const std::string& text, const std::string& fontPath, const Color4f& color1, const Color4f& color2, int textSize)
{
	m_pUnselectedTextrue = new Texture{ text, fontPath, textSize, color1 };
	m_pSelectedTexture = new Texture{ text, fontPath, textSize, color2 };
}

MenuWord::~MenuWord()
{
	delete m_pUnselectedTextrue;
	m_pUnselectedTextrue = nullptr;
	delete m_pSelectedTexture;
	m_pSelectedTexture = nullptr;
}

void MenuWord::Draw(const Point2f& pos, bool selected)
{
	glPushMatrix();
	glTranslatef(pos.x - m_pSelectedTexture->GetWidth() / 2, pos.y, 0);
	if (selected)m_pSelectedTexture->Draw();
	else m_pUnselectedTextrue->Draw();
	glPopMatrix();
}

float MenuWord::GetHeight()
{
	return m_pSelectedTexture->GetHeight();
}

float MenuWord::GetWidth()
{
	return m_pSelectedTexture->GetWidth();
}