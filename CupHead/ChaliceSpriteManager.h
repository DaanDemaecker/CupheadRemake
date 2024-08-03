#pragma once
#include "ChaliceAvatar.h"
#include "Enums.h"
#include <vector>
class SpriteSheet;
class ShootSpriteSheet;

class ChaliceSpriteManager final
{
public:
	explicit ChaliceSpriteManager(Enums::Character character);
	~ChaliceSpriteManager();

	ChaliceSpriteManager(ChaliceSpriteManager& rhs) = delete;
	ChaliceSpriteManager(ChaliceSpriteManager&& rhs) = delete;

	ChaliceSpriteManager& operator=(ChaliceSpriteManager& rhs) = delete;
	ChaliceSpriteManager& operator=(ChaliceSpriteManager&& rhs) = delete;


	void Draw(ChaliceAvatar::State state, const Point2f& pos, bool facingRight);
	void Update(float elapsedSec, ChaliceAvatar::State state, float yVelocity);
	void UpdateAngle(int angle);
	bool GetFinishedCycle(ChaliceAvatar::State state);
	void ResetCycle(ChaliceAvatar::State state);
	int GetCrFrame(ChaliceAvatar::State state);

	void SetGhostPos(const Point2f& pos);

	void ShootBullet(ChaliceAvatar::State state);

private:
	std::map<ChaliceAvatar::State, SpriteSheet*> m_pSprites;

	SpriteSheet* m_pGhostSpriteSheet;
	ShootSpriteSheet* m_pJumpShootHead;

	int m_Angle{};

	Point2f m_GhostPos{};
	const float m_GhostSpeed{ 100.f };
};

