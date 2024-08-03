#pragma once
#include "Texture.h"

class BulletSpriteSheet final
{
//Loopspritesheet with a different draw function to make calculations easier

public:
	BulletSpriteSheet(Texture* texturePtr, int nrCols, int nrRows, int nrFrames, int loopStart, float frameSpeed = 1 / 20.f);
	void Draw(const Point2f& pos, float angle) const;
	void DrawTransparent(const Point2f& pos, float angle) const;
	void Update(float elapsedSec);

	int GetCrFrame();

private:
	Texture* m_pTexture;

	const float m_FrameSpeed;
	const int m_NrCols;
	const int m_NrRows;
	const int m_NrFrames;

	int m_LoopStart;

	int m_CrFrame{};
	float m_Timer{};

	void NextFrame();
};

