#pragma once
#include <string>
#include "ParticleSoundeffectManager.h"
class LoopSpriteSheet;
class Level;

class Object final
{
public:
	explicit Object(std::string textpath, ParticleSoundeffectManager::ParticleType deathParticle, ParticleSoundeffectManager::SFXType dathSFX, int nrFrames, int loopStart, bool damaging, bool parryable = false, bool parryDestroys = false);
	~Object();

	Object(Object& rhs) = delete;
	Object(Object&& rhs) = delete;
	Object& operator=(Object& rhs) = delete;
	Object& operator=(Object&& rhs) = delete;

	virtual void Draw();
	void Update(float elapsedSec, const Level* level);
	virtual void SetObject(const Point2f& center, float angle, float speed);

	bool CheckParry(const Rectf& shape);
	bool CeckHit(const Rectf& shape, bool parry);

	Point2f GetCenter();
	bool GetActivated();
private:
	float m_Angle{};
	float m_Speed{};

	bool m_Damaging;
	bool m_Parryable;
	bool m_ParryDestroys;

protected:
	ParticleSoundeffectManager::ParticleType  m_DeathParticle;
	ParticleSoundeffectManager::SFXType m_DeathSFX;

	LoopSpriteSheet* m_pSpriteSheet;
	Point2f m_Center{};
	float m_Radius;
	bool m_Activated{ false };
};

