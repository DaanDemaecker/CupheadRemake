#pragma once
#include "Enums.h"
class Texture;

class Hud final
{
public:
	Hud(Enums::Character cupHead, bool canSwapWeapon, bool normalHud);
	~Hud();

	Hud(Hud& rhs) = delete;
	Hud(Hud&& rhs) = delete;

	Hud& operator=(Hud& rhs) = delete;
	Hud& operator=(Hud&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec, int health, float super);

	void SwapWeapon(Enums::BulletType bulletType);
	void SwapWeapon(Enums::PlaneBulletType bulletType);

private:
	Texture* m_pHealthTexture;
	Texture* m_pCardTexture;
	Texture* m_pSwapWeaponTexture;
	Texture* m_pBulletIconsTexture;

	const float m_Border{ 10.f };

	float m_SwapWeaponTimer{};

	bool m_DrawBulletIcon{ false };
	bool m_BulletIconGoingUp{ true };
	float m_BulletIconPercentage{};
	float m_BulletIconTimer{};

	Enums::BulletType m_BulletType{};
	Enums::PlaneBulletType m_PlaneBulleTType{};

	bool m_NormalHud;

	int m_CrHealthFrame{};
	float m_HealthTimer{};
	const int m_HealtCols{6};
	const int m_HealthRows{ 2 };
	const int m_HealthFrames{11};
	const float m_FrameSpeed{1/15.f};

	float m_Super{};

	const int m_NrCardFrames{6};
	int m_CrCardFrame{};
	bool m_Flipping{ false };
	float m_FlipTimer{};

	void DrawCards() const;
};

