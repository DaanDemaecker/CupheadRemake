#pragma once
#include "OneCycleSpriteSheet.h"
#include <string>

class ParticleSpriteSheet final
{
public:
	ParticleSpriteSheet(Texture* texture,int nrCols, int nrRows, int nrFrames, const Point2f& pos, bool facingRight = true, float frameSpeed = 1 / 20.f);
	ParticleSpriteSheet(Texture* texture, int nrCols, int nrRows, int nrFrames, const Point2f& pos, float angle, float frameSpeed = 1 / 20.f);
	
	ParticleSpriteSheet(ParticleEntry* entry, const Point2f& pos, bool facingRight = true);
	ParticleSpriteSheet(ParticleEntry* entry, const Point2f& pos, float angle);

	~ParticleSpriteSheet();

	ParticleSpriteSheet(ParticleSpriteSheet& rhs) = delete;
	ParticleSpriteSheet(ParticleSpriteSheet&& rhs) = delete;

	ParticleSpriteSheet& operator=(ParticleSpriteSheet& rhs) = delete;
	ParticleSpriteSheet& operator=(ParticleSpriteSheet&& rhs) = delete;

	void Draw() const;

	void Update(float elapsedSec);
	bool GetFinishedCycle();
	void ResetCycle();

private:
	const Point2f m_Pos;
	const bool m_FacingRight;
	const float m_Angle;

	bool m_FinishedCycle{false};
	const float m_FrameSpeed;
	const int m_NrCols;
	const int m_NrRows;
	const int m_NrFrames;
	Texture* m_pTexture;

	
	int m_CrFrame{};
	float m_Timer{};

	void NextFrame();
};