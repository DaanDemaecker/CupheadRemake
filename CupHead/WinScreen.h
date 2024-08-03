#pragma once
#include "Enums.h"
class Texture;
class LoopSpriteSheet;
class OscillatingSpriteSheet;
class MenuWord;
#include <vector>

class WinScreen final
{
public: 
	WinScreen(const Rectf& window, const PlayerEntry& player);

	~WinScreen();

	WinScreen(WinScreen& other) = delete;
	WinScreen(WinScreen&& other) = delete;

	WinScreen& operator=(WinScreen& other) = delete;
	WinScreen& operator=(WinScreen&& other) = delete;


	void Draw() const;
	void Update(float elapsedSec);

	bool ShouldGoToMenu();

private:

	const Rectf m_Window;

	Texture* m_pBackGroundTexture;
	Texture* m_pTheResultTexture;
	Texture* m_pTitleTexture;
	
	OscillatingSpriteSheet* m_pCharacterSpritesheet;
	Texture* m_pCharacterTitle;

	Texture* m_pBoardTexture;

	LoopSpriteSheet* m_pStar1Texture;
	LoopSpriteSheet* m_pStar2Texture;

	LoopSpriteSheet* m_pCirlceTexture;
	Texture* m_pGradeTexture;

	float m_Angle{};
	const float m_AngleSpeed{ 5 };

	float m_Timer{};
	const float m_Time{90};

	std::vector<MenuWord*> m_pNumberTextures;
	Texture* m_pColonTexture;
	Texture* m_pSlashTexture;

#pragma region Scores
	float m_CounterTimer{};
	float m_CounterTime{ .05f };

	bool m_TimeDone{ false };
	int m_DisplayTime{};
	int m_TargetTime;
	const int m_MaxTime{ 120 };

	bool m_HpBonusDone{false};
	int m_DisplayHpBonus{};
	int m_HpBonus;
	const int m_HpBonusMax{ 3 };

	bool m_ParryDone{ false };
	int m_DisplayParry{};
	int m_Parry;
	const int m_ParryMax{ 3 };

	bool m_SuperMeterDone{false};
	int m_DisplaySuperMeter{};
	int m_SuperMeter;
	const int m_SuperMeterMax{ 6 };

	bool m_Star1Done{ false };
	bool m_Star2Done{ false };
#pragma endregion Scores

	void ReadFromFile();

	void DrawCharacterAndBoard() const;
	void DrawBoard() const;

	void IncreaseNumber();

	int GetTime(int time, int position) const;
	std::string GetGrade();
};

