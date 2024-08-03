#pragma once
class SpriteSheet;
class OneCycleSpriteSheet;

class ChargeBulletManager final
{
public:
	ChargeBulletManager();
	~ChargeBulletManager();

	void Draw() const;
	void Update(float elapsedSec, bool shooting, const Point2f& pos);

	bool ShouldShootSmall();
	bool ShouldShootBig();

	void StopShoot();

private:
	SpriteSheet* m_pSmallCharging1Texture;
	OneCycleSpriteSheet* m_pSmallCharging2Texture;
	SpriteSheet* m_pBigCharging1Texture;
	SpriteSheet* m_pBigCharging2Texture;

	bool m_Shooting{false};
	Point2f m_Pos{};

	int m_SmallChargingLoops{};

	bool m_Charged{ false };

	bool m_ShouldShootSmall{ false };
	bool m_ShouldShootBig{ false };
};

