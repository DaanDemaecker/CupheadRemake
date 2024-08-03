#pragma once
class Texture;
#include <vector>
#include <map>
#include "Enums.h"

class EquipmentCard final
{
public:
	EquipmentCard(const Rectf& window, PlayerEntry player);
	~EquipmentCard();

	EquipmentCard(EquipmentCard& other) = delete;
	EquipmentCard(EquipmentCard&& other) = delete;

	EquipmentCard& operator=(EquipmentCard& other) = delete;
	EquipmentCard& operator=(EquipmentCard&& other) = delete;

	void Draw(Enums::Character character) const;

	void Update(float elapsedSec, PlayerEntry& player);

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();
	void BackSpace();

	bool ShouldGoBack();

private:
	enum class State
	{
		Main,
		ShotA,
		ShotB,
		Charm
	};
	State m_State{State::Main};

	const Rectf m_Window{};

	Texture* m_pCardTexture1CupHead;
	Texture* m_pCardTexture1MugMan;
	Texture* m_pCardTexture2CupHead;
	Texture* m_pCardTexture2MugMan;

	Texture* m_pSelectionTexture;

	Texture* m_pBulletIcons;
	Texture* m_pBulletIconsGrey;
	Texture* m_pCharmIcons;
	Texture* m_pEmptyTexture;
	Texture* m_pCircleTexture;
	Texture* m_pEquippedTexture;

	std::vector<Texture*> m_pBulletTitles;
	std::vector<Texture*> m_pCharmTitles;
	std::vector<Texture*> m_pCardTitles;
	Texture* m_pNotAvailableText;

	Texture* m_pBulletDescriptions;
	Texture* m_pCharmDescriptions;

	PlayerEntry m_PlayerEntry;
	int m_CrCol{};
	const int m_ColAmount{3};
	float m_Timer{};
	const float m_FrameRate{ 1 / 15.f };

	std::map<Enums::BulletType, Point2f> m_BulletPositions;
	std::map<Enums::Charm, Point2f> m_CharmPositions;

	int m_Selection{};

	bool m_ShouldGoBack{ false };

	void InitializeBulletPositions();
	void InitializeCharmPositions();


	void DrawMainCard(Enums::Character character) const;
	void DrawShotACard(Enums::Character character) const;
	void DrawShotBCard(Enums::Character character) const;
	void DrawCharmCard(Enums::Character character) const;

	void DrawShotIcon(Enums::BulletType type, Enums::BulletType other, const Point2f& pos, Rectf& srcRect) const;
	void DrawCharmIcon(Enums::Charm, const Point2f& pos, Rectf& srcRect) const;
};

