#include "pch.h"
#include "EquipmentCard.h"
#include "Texture.h"
#include "ParticleSoundeffectManager.h"
#include "utils.h"
#include "Avatar.h"
#include "BulletManager.h"

extern ParticleSoundeffectManager* g_pSFXManager;

EquipmentCard::EquipmentCard(const Rectf& window, PlayerEntry player)
	:m_Window{ window }, m_PlayerEntry{player}
{
	std::string textPath{ "./resources/images/ui/equipment card/" };

	m_pCardTexture1CupHead = new Texture{textPath + "Card_Cuphead.png"};
	m_pCardTexture1MugMan = new Texture{ textPath + "Card_MugMan.png" };

	m_pCardTexture2CupHead = new Texture{ textPath + "Card2_Cuphead.png" };
	m_pCardTexture2MugMan = new Texture{ textPath + "Card2_MugMan.png" };

	m_pSelectionTexture = new Texture{textPath + "Selection.png"};

	m_pBulletIcons = new Texture{textPath + "Bullet_Icons.png"};
	m_pBulletIconsGrey = new Texture{ textPath + "Bullet_Icons_Grey.png" };
	m_pCharmIcons = new Texture{textPath + "Charm_Icons.png"};
	m_pEmptyTexture = new Texture{ textPath + "Empty.png" };
	m_pCircleTexture = new Texture{ textPath + "Circle.png" };
	m_pEquippedTexture = new Texture{ textPath + "Equipped.png" };

	m_pBulletDescriptions = new Texture{textPath + "Bullet_Descriptions.png"};
	m_pCharmDescriptions = new Texture{ textPath + "Charm_Descriptions.png" };

	textPath = "./resources/fonts/Memphis-Medium.otf";

	Color4f color{ 0.f, 0.f, 0.f, 1.f };

	m_pBulletTitles.push_back(new Texture{ "Pea Shooter", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "SpreadShot", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Roundabout", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Lobber", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Charge", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Chaser", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "CrackShot", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Converge", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Twist-Up", textPath, 40, color });
	m_pBulletTitles.push_back(new Texture{ "Empty", textPath, 40, color });

	m_pCharmTitles.push_back(new Texture{ "Heart", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Coffee", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Smoke Bomb", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Parry Sugar", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Twin Hearts", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Whetstone", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Heart Ring", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Astral Cookie", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Broken Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Cursed Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Cursed Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Cursed Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Cursed Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Divine Relic", textPath, 40, color });
	m_pCharmTitles.push_back(new Texture{ "Empty", textPath, 40, color });

	m_pNotAvailableText = new Texture{ "Not Available", textPath, 40, color };

	color = Color4f{ 0.8470588235294118f,0.7764705882352941f,0.6901960784313725f, 1.f };
	m_pCardTitles.push_back(new Texture{ "Shot-A", textPath, 60, color });
	m_pCardTitles.push_back(new Texture{ "Shot-B", textPath, 60, color });
	m_pCardTitles.push_back(new Texture{ "Charm", textPath, 60, color });

	InitializeBulletPositions();
	InitializeCharmPositions();
}

EquipmentCard::~EquipmentCard()
{
	delete m_pCardTexture1CupHead;
	m_pCardTexture1CupHead = nullptr;
	delete m_pCardTexture1MugMan;
	m_pCardTexture1MugMan = nullptr;
	delete m_pCardTexture2CupHead;
	m_pCardTexture2CupHead = nullptr;
	delete m_pCardTexture2MugMan;
	m_pCardTexture2MugMan = nullptr;

	delete m_pSelectionTexture;
	m_pSelectionTexture = nullptr;
	delete m_pBulletIcons;
	m_pBulletIcons = nullptr;
	delete m_pBulletIconsGrey;
	m_pBulletIconsGrey = nullptr;
	delete m_pCharmIcons;
	m_pCharmIcons = nullptr;
	delete m_pEmptyTexture;
	m_pEmptyTexture = nullptr;
	delete m_pCircleTexture;
	m_pCircleTexture = nullptr;
	delete m_pEquippedTexture;
	m_pEquippedTexture = nullptr;

	delete m_pBulletDescriptions;
	m_pBulletDescriptions = nullptr;
	delete m_pCharmDescriptions;
	m_pCharmDescriptions = nullptr;

	delete m_pNotAvailableText;
	m_pNotAvailableText = nullptr;
	for (int i{}; i < m_pBulletTitles.size(); i++)
	{
		delete m_pBulletTitles[i];
		m_pBulletTitles[i] = nullptr;
	}
	m_pBulletTitles.clear();
	for (int i{}; i < m_pCharmTitles.size(); i++)
	{
		delete m_pCharmTitles[i];
		m_pCharmTitles[i] = nullptr;
	}
	m_pCharmTitles.clear();
	for (int i{}; i < m_pCardTitles.size(); i++)
	{
		delete m_pCardTitles[i];
		m_pCardTitles[i] = nullptr;
	}
	m_pCardTitles.clear();
}

void EquipmentCard::InitializeBulletPositions()
{
	m_BulletPositions[Enums::BulletType::PeaShooter] = Point2f{ 0,0 };
	m_BulletPositions[Enums::BulletType::SpreadShot] = Point2f{ 1,0 };
	m_BulletPositions[Enums::BulletType::Roundabout] = Point2f{ 2,0 };
	m_BulletPositions[Enums::BulletType::Lobber] = Point2f{ 3,0 };
	m_BulletPositions[Enums::BulletType::Charge] = Point2f{ 4,0 };
	m_BulletPositions[Enums::BulletType::Chaser] = Point2f{ .5f, -1 };
	m_BulletPositions[Enums::BulletType::CrackShot] = Point2f{ 1.5f, -1 };
	m_BulletPositions[Enums::BulletType::Converge] = Point2f{ 2.5f ,-1 };
	m_BulletPositions[Enums::BulletType::TwistUp] = Point2f{ 3.5f ,-1 };
}

void EquipmentCard::InitializeCharmPositions()
{
	m_CharmPositions[Enums::Charm::Heart] = Point2f{ 0,0 };
	m_CharmPositions[Enums::Charm::Coffee] = Point2f{ 1,0 };
	m_CharmPositions[Enums::Charm::Smoke_Bomb] = Point2f{ 2,0 };
	m_CharmPositions[Enums::Charm::PSugar] = Point2f{ 3,0 };
	m_CharmPositions[Enums::Charm::TwinHearts] = Point2f{ 4,0 };
	m_CharmPositions[Enums::Charm::Whetstone] = Point2f{ .5f, -1 };
	m_CharmPositions[Enums::Charm::HeartRing] = Point2f{ 1.5f, -1 };
	m_CharmPositions[Enums::Charm::AstralCookie] = Point2f{ 2.5f ,-1 };

	if(m_PlayerEntry.cursedLevel == 0)m_CharmPositions[Enums::Charm::BrokenRelic] = Point2f{ 3.5f ,-1 };
	else if (m_PlayerEntry.cursedLevel == 1)m_CharmPositions[Enums::Charm::CursedRelic1] = Point2f{ 3.5f ,-1 };
	else if (m_PlayerEntry.cursedLevel == 2)m_CharmPositions[Enums::Charm::CursedRelic2] = Point2f{ 3.5f ,-1 };
	else if (m_PlayerEntry.cursedLevel == 3)m_CharmPositions[Enums::Charm::CursedRelic3] = Point2f{ 3.5f ,-1 };
	else if (m_PlayerEntry.cursedLevel == 4)m_CharmPositions[Enums::Charm::CursedRelic4] = Point2f{ 3.5f ,-1 };
	else if (m_PlayerEntry.cursedLevel == 5)m_CharmPositions[Enums::Charm::DivineRelic] = Point2f{ 3.5f ,-1 };
}

bool EquipmentCard::ShouldGoBack()
{
	if (m_ShouldGoBack)
	{
		m_ShouldGoBack = false;
		return true;
	}
	return false;
}

void EquipmentCard::Draw(Enums::Character character) const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.6f });
	utils::FillRect(m_Window);

	glPushMatrix();
	glTranslatef(450, 20, 0);
	glRotatef(3, 0, 0, 1);
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		DrawMainCard(character);
		break;
	case EquipmentCard::State::ShotA:
		DrawShotACard(character);
		break;
	case EquipmentCard::State::ShotB:
		DrawShotBCard(character);
		break;
	case EquipmentCard::State::Charm:
		DrawCharmCard(character);
		break;
	default:
		break;
	}

	glPopMatrix();
}

void EquipmentCard::Update(float elapsedSec, PlayerEntry& player)
{
	player = m_PlayerEntry;

	m_Timer += elapsedSec;
	if (m_Timer >= m_FrameRate)
	{
		m_Timer -= m_FrameRate;
		++m_CrCol %= m_ColAmount;
	}
}

void EquipmentCard::Space()
{

	switch (m_State)
	{
	case EquipmentCard::State::Main:
		if (m_Selection == 0)
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
			m_Selection = int(m_PlayerEntry.bulletType1) % int(Enums::BulletType::none);
			m_State = State::ShotA;
		}
		else if (m_Selection == 1)
		{
			m_Selection = int(m_PlayerEntry.bulletType2) % int(Enums::BulletType::none);
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
			m_State = State::ShotB;
		}
		else if(m_Selection == 2)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Locked);
		else if (m_Selection == 3)
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
			m_Selection = int(m_PlayerEntry.charm);
			if (m_Selection > 8) m_Selection = 8;
			m_State = State::Charm;
		}
		else if(m_Selection == 4)g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Locked);
		break;
	case EquipmentCard::State::ShotA:
		if (m_Selection != int(m_PlayerEntry.bulletType2))
		{
			m_PlayerEntry.bulletType1 = static_cast<Enums::BulletType>(m_Selection);
		}
		else
		{
			m_PlayerEntry.bulletType2 = m_PlayerEntry.bulletType1;
			m_PlayerEntry.bulletType1 = static_cast<Enums::BulletType>(m_Selection);
		}
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		break;
	case EquipmentCard::State::ShotB:
		if (m_Selection != int(m_PlayerEntry.bulletType1))
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
			m_PlayerEntry.bulletType2 = static_cast<Enums::BulletType>(m_Selection);
		}
		else if(m_PlayerEntry.bulletType2 != Enums::BulletType::none)
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
			m_PlayerEntry.bulletType1 = m_PlayerEntry.bulletType2;
			m_PlayerEntry.bulletType2 = static_cast<Enums::BulletType>(m_Selection);
		}
		else g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Locked);
		break;
	case EquipmentCard::State::Charm:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);

		if(m_Selection < 8)	m_PlayerEntry.charm = static_cast<Enums::Charm>(m_Selection);
		else m_PlayerEntry.charm = static_cast<Enums::Charm>(m_Selection + m_PlayerEntry.cursedLevel);
		break;
	default:
		break;
	}
}

void EquipmentCard::Escape()
{
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		m_ShouldGoBack = true;
		break;
	case EquipmentCard::State::ShotA:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
		m_State = State::Main;
		m_Selection = 0;
		break;
	case EquipmentCard::State::ShotB:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
		m_State = State::Main;
		m_Selection = 1;
		break;
	case EquipmentCard::State::Charm:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Cardflip);
		m_State = State::Main;
		m_Selection = 3;
		break;
	default:
		break;
	}
}

void EquipmentCard::Up()
{
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		break;
	case EquipmentCard::State::ShotA:
	case EquipmentCard::State::ShotB:
	case EquipmentCard::State::Charm:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		m_Selection += 5;
		m_Selection %= 9;
		break;
	default:
		break;
	}
}

void EquipmentCard::Right()
{
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		++m_Selection %= 5;
		break;
	case EquipmentCard::State::ShotA:
	case EquipmentCard::State::ShotB:
	case EquipmentCard::State::Charm:
		++m_Selection %= 9;
		break;
	default:
		break;
	}
}

void EquipmentCard::Down()
{
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		break;
	case EquipmentCard::State::ShotA:;
	case EquipmentCard::State::ShotB:
	case EquipmentCard::State::Charm:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
		m_Selection += 5;
		m_Selection %= 9;
		break;
	default:
		break;
	}
}

void EquipmentCard::Left()
{
	g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Move);
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		if (--m_Selection < 0)m_Selection = 4;
		break;
	case EquipmentCard::State::ShotA:
	case EquipmentCard::State::ShotB:
	case EquipmentCard::State::Charm:
		--m_Selection %= 9;
		if (m_Selection < 0)m_Selection = 8;
		break;
	default:
		break;
	}
}

void EquipmentCard::BackSpace()
{
	switch (m_State)
	{
	case EquipmentCard::State::Main:
		if (m_Selection == 1)
		{
			m_PlayerEntry.bulletType2 = Enums::BulletType::none;
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		}
		else if (m_Selection == 3)
		{
			g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
			m_PlayerEntry.charm = Enums::Charm::None;
		}
		else g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Locked);
		break;
	case EquipmentCard::State::ShotA:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Locked);
		break;
	case EquipmentCard::State::ShotB:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		m_PlayerEntry.bulletType2 = Enums::BulletType::none;
		break;
	case EquipmentCard::State::Charm:
		g_pSFXManager->PlaySFX(ParticleSoundeffectManager::SFXType::Menu_Select);
		m_PlayerEntry.charm = Enums::Charm::None;
		break;
	default:
		break;
	}
}

void EquipmentCard::DrawMainCard(Enums::Character character) const
{
	if (character == Enums::Character::CupHead)m_pCardTexture1CupHead->Draw();
	else m_pCardTexture1MugMan->Draw();

	Rectf srcRect{};

	Point2f pos{ 57, 125 };
	Point2f selectionOffset{ 11.f, 10.f };

	utils::CalculateSrcRect(m_pBulletIcons, srcRect, m_ColAmount, 9, int(m_PlayerEntry.bulletType1)*m_ColAmount + m_CrCol);
	m_pBulletIcons->Draw(pos, srcRect);
	if (m_Selection == 0)m_pSelectionTexture->Draw(Point2f{pos.x - selectionOffset.x, pos.y - selectionOffset.y});

	pos.x += 99;
	utils::CalculateSrcRect(m_pBulletIcons, srcRect, m_ColAmount, 9, int(m_PlayerEntry.bulletType2) * m_ColAmount + m_CrCol);
	if (m_PlayerEntry.bulletType2 != Enums::BulletType::none)m_pBulletIcons->Draw(pos, srcRect);
	else m_pEmptyTexture->Draw(Point2f{pos.x + 1.f, pos.y + 2.f});
	if (m_Selection == 1)m_pSelectionTexture->Draw(Point2f{ pos.x - selectionOffset.x, pos.y - selectionOffset.y });

	pos.x += 102;
	m_pEmptyTexture->Draw(Point2f{ pos.x + 1.f, pos.y + 2.f });
	if (m_Selection == 2)m_pSelectionTexture->Draw(Point2f{ pos.x - selectionOffset.x, pos.y - selectionOffset.y });

	pos.x += 97;
	utils::CalculateSrcRect(m_pCharmIcons, srcRect, m_ColAmount, 14, int(m_PlayerEntry.charm)*m_ColAmount + m_CrCol);
	if(m_PlayerEntry.charm != Enums::Charm::None) m_pCharmIcons->Draw(pos, srcRect);
	else m_pEmptyTexture->Draw(Point2f{ pos.x + 1.f, pos.y + 2.f });
	if (m_Selection == 3)m_pSelectionTexture->Draw(Point2f{ pos.x - selectionOffset.x, pos.y - selectionOffset.y });

	pos.x += 100;
	if (m_Selection == 4)m_pSelectionTexture->Draw(Point2f{ pos.x - selectionOffset.x, pos.y - selectionOffset.y });

	pos.x = m_pCardTexture1CupHead->GetWidth() / 2.f;
	pos.y = 22;

	if (m_Selection == 2 || m_Selection == 4)
	{
		m_pNotAvailableText->Draw(Point2f{ pos.x - m_pNotAvailableText->GetWidth() / 2, pos.y });
	}
	else if (m_Selection == 0)
	{
		m_pBulletTitles[int(m_PlayerEntry.bulletType1)]->Draw(Point2f{pos.x - m_pBulletTitles[int(m_PlayerEntry.bulletType1)]->GetWidth() / 2, pos.y});
	}
	else if (m_Selection == 1)
	{
		m_pBulletTitles[int(m_PlayerEntry.bulletType2)]->Draw(Point2f{ pos.x - m_pBulletTitles[int(m_PlayerEntry.bulletType2)]->GetWidth() / 2, pos.y });
	}
	else if (m_Selection == 3)
	{
		m_pCharmTitles[int(m_PlayerEntry.charm)]->Draw(Point2f{ pos.x - m_pCharmTitles[int(m_PlayerEntry.charm)]->GetWidth() / 2, pos.y });
	}
}

void EquipmentCard::DrawShotACard(Enums::Character character) const
{
	if (character == Enums::Character::CupHead)m_pCardTexture2CupHead->Draw();
	else m_pCardTexture2MugMan->Draw();

	Rectf srcRect{};

	Point2f pos{ m_pCardTexture2CupHead->GetWidth() / 2 - m_pCardTitles[2]->GetWidth() / 2, m_pCardTexture2CupHead->GetHeight() - 130 };
	m_pCardTitles[0]->Draw(pos);
	Point2f leftTopPos{ 40, 390 };
	float distance{ 100 };


	for (std::pair<Enums::BulletType, Point2f> pair : m_BulletPositions)
	{
		pos = Point2f{ leftTopPos.x + pair.second.x * distance, leftTopPos.y + pair.second.y * distance };
		m_pCircleTexture->Draw(pos);
		pos = Point2f{ pos.x + 8, pos.y + 8 };
		DrawShotIcon(pair.first, m_PlayerEntry.bulletType2, pos, srcRect);

		if (m_Selection == int(pair.first))m_pSelectionTexture->Draw(Point2f{ pos.x - 11, pos.y - 11 });

		if (m_PlayerEntry.bulletType1 == pair.first)m_pEquippedTexture->Draw(Point2f{ pos.x + 40, pos.y + 40 });
	}

	pos = Point2f{ 30, 30 };
	utils::CalculateSrcRect(m_pBulletDescriptions, srcRect, 3, 3, m_Selection);
	m_pBulletDescriptions->Draw(pos, srcRect);
}

void EquipmentCard::DrawShotBCard(Enums::Character character) const
{
	if (character == Enums::Character::CupHead)m_pCardTexture2CupHead->Draw();
	else m_pCardTexture2MugMan->Draw();

	Rectf srcRect{};

	Point2f pos{ m_pCardTexture2CupHead->GetWidth() / 2 - m_pCardTitles[2]->GetWidth() / 2, m_pCardTexture2CupHead->GetHeight() - 130 };
	m_pCardTitles[0]->Draw(pos);
	Point2f leftTopPos{ 40, 390 };
	float distance{ 100 };


	for (std::pair<Enums::BulletType, Point2f> pair : m_BulletPositions)
	{
		pos = Point2f{ leftTopPos.x + pair.second.x * distance, leftTopPos.y + pair.second.y * distance };
		m_pCircleTexture->Draw(pos);
		pos = Point2f{ pos.x + 8, pos.y + 8 };
		DrawShotIcon(pair.first, m_PlayerEntry.bulletType1, pos, srcRect);

		if (m_Selection == int(pair.first))m_pSelectionTexture->Draw(Point2f{ pos.x - 11, pos.y - 11 });

		if (m_PlayerEntry.bulletType2 == pair.first)m_pEquippedTexture->Draw(Point2f{ pos.x + 40, pos.y + 40 });
	}

	pos = Point2f{ 30, 30 };
	utils::CalculateSrcRect(m_pBulletDescriptions, srcRect, 3, 3, m_Selection);
	m_pBulletDescriptions->Draw(pos, srcRect);
}

void EquipmentCard::DrawCharmCard(Enums::Character character) const
{
	if (character == Enums::Character::CupHead)m_pCardTexture2CupHead->Draw();
	else m_pCardTexture2MugMan->Draw();

	Rectf srcRect{};

	Point2f pos{ m_pCardTexture2CupHead->GetWidth() / 2 - m_pCardTitles[2]->GetWidth() / 2, m_pCardTexture2CupHead->GetHeight() - 130 };
	m_pCardTitles[2]->Draw(pos);
	Point2f leftTopPos{ 40, 390 };
	float distance{ 100 };


	for (std::pair<Enums::Charm, Point2f> pair : m_CharmPositions)
	{
		pos = Point2f{ leftTopPos.x + pair.second.x * distance, leftTopPos.y + pair.second.y * distance };
		m_pCircleTexture->Draw(pos);
		pos = Point2f{ pos.x + 8, pos.y + 8 };
		DrawCharmIcon(pair.first, pos, srcRect);

		if (m_Selection < 8)
		{
			if (m_Selection == int(pair.first))m_pSelectionTexture->Draw(Point2f{ pos.x - 11, pos.y - 11 });
		}
		else
		{
			if (m_Selection == int(pair.first) - m_PlayerEntry.cursedLevel)m_pSelectionTexture->Draw(Point2f{ pos.x - 11, pos.y - 11 });
		}
		if (m_PlayerEntry.charm == pair.first)m_pEquippedTexture->Draw(Point2f{ pos.x + 40, pos.y + 40 });
	}

	pos = Point2f{ 30, 30 };
	if(m_Selection < 8)utils::CalculateSrcRect(m_pCharmDescriptions, srcRect, 3, 5, m_Selection);
	else utils::CalculateSrcRect(m_pCharmDescriptions, srcRect, 3, 5, m_Selection + m_PlayerEntry.cursedLevel);
	m_pCharmDescriptions->Draw(pos, srcRect);
}

void EquipmentCard::DrawShotIcon(Enums::BulletType type, Enums::BulletType other, const Point2f& pos, Rectf& srcRect) const
{
	utils::CalculateSrcRect(m_pBulletIcons, srcRect, m_ColAmount, 9, int(type)*m_ColAmount + m_CrCol);
	if (other == type)
	{
		m_pBulletIconsGrey->Draw(pos, srcRect);
	}
	else m_pBulletIcons->Draw(pos, srcRect);
}

void EquipmentCard::DrawCharmIcon(Enums::Charm charm, const Point2f& pos, Rectf& srcRect) const
{
	utils::CalculateSrcRect(m_pCharmIcons, srcRect, m_ColAmount, 14, int(charm) * m_ColAmount + m_CrCol);
	m_pCharmIcons->Draw(pos, srcRect);
}
