#pragma once
#include <vector>
#include "Enums.h"
class Texture;
class SpriteSheet;
class MenuWord;

class LoseScreen final
{
public:
	LoseScreen(const Rectf& window, Enums::Character cuphead, LoseScreenData loseScreenData);
	~LoseScreen();

	void Draw() const;
	void Update(float elapsedSec);

	void Up();
	void Down();
	void Space();

	bool ShouldReset();
	bool ShouldExitToOverworld();

private:
	const Rectf m_Window;

	Texture* m_pCardTexture;
	Texture* m_pMugShotTexture;
	Texture* m_pBarTexture;
	Texture* m_pNodeTexture;

	SpriteSheet* m_pPlayerTexture;

	std::vector<MenuWord*> m_pWords;
	int m_Selected{};

	float* m_pNodes;
	const int m_NodeAmount;
	const float m_Distance;
	float m_DisplayDistance{};

	bool m_Secret;

	bool m_Reset{ false };
	bool m_ExitMenu{ false };

	void DrawCard() const;
	void DrawBar() const;

	float GetPlayerPos() const;
};

