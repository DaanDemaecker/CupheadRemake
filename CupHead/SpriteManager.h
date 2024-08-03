#pragma once
#include "NormalAvatar.h"
#include "Enums.h"
#include <vector>
class SpriteSheet;

class SpriteManager final
{
public:
	explicit SpriteManager(Enums::Character Cuphead);
	~SpriteManager();

	SpriteManager(SpriteManager& rhs) = delete;
	SpriteManager(SpriteManager&& rhs) = delete;

	SpriteManager& operator=(SpriteManager& rhs) = delete;
	SpriteManager& operator=(SpriteManager&& rhs) = delete;


	void Draw(NormalAvatar::State state, const Point2f& pos, bool facingRight);
	void Update(float elapsedSec, NormalAvatar::State state);
	void UpdateAngle(int angle);
	bool GetFinishedCycle(NormalAvatar::State state);
	void ResetCycle(NormalAvatar::State state);
	int GetCrFrame(NormalAvatar::State state);

	void SetGhostPos(const Point2f& pos);

	void ShootBullet(NormalAvatar::State state);

private:
	std::map<NormalAvatar::State, SpriteSheet*> m_pSprites;

	SpriteSheet* m_pGhostSpriteSheet;

	int m_Angle{};

	Point2f m_GhostPos{};
	const float m_GhostSpeed{ 100.f };
};

