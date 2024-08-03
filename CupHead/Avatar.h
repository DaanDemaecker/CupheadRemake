#pragma once
class Level;
class SpriteManager;
class ParticleSoundeffectManager;
class Hud;
class Bullet;
class BigBullet;
class ObjectManager;
class Shadow;

#include "BulletManager.h"
#include "Enums.h"
#include "Vector2f.h"
#include <vector>
#include <map>

class Avatar
{
public:
	explicit Avatar(Controller* controller);
	virtual ~Avatar();

	Avatar(Avatar& rhs) = delete;
	Avatar(Avatar&& rhs) = delete;

	Avatar& operator=(Avatar& rhs) = delete;
	Avatar& operator=(Avatar&& rhs) = delete;

	virtual void SaveToFile() = 0;

	virtual void Update(float elapsedSec, const Level* level, ObjectManager* objectManager) = 0;
	virtual void Draw() const = 0;
	virtual void DrawHud() const = 0;

	virtual void SetShooting(bool shooting) = 0;

	virtual void Hit() = 0;
	virtual void Parry() = 0;
	virtual void SpecialAtk() = 0;
	virtual void IncreaseSuper(float amount) = 0;

	virtual bool IsDead() const = 0;
	virtual Rectf& GetShape() = 0;
	virtual bool GetParry() const = 0;
	virtual Vector2f& GetVelocity() = 0;
	virtual bool CanGetHit() const = 0;
	virtual float GetParryDamage() const = 0;
	virtual std::vector<Bullet*>& GetBullets() = 0;
	virtual std::vector<BigBullet*>& GetBigBullets() = 0;

	void CharmSuperCharge(float elapsedSec, float& super, Enums::Charm charm);

	bool ShouldAxeParry(Enums::Charm charm, int parryAmount);

	bool RelicEquiped(Enums::Charm charm);

	void Input(bool checkKeyboard);

protected:
	Controller* m_pController;

	std::map<Enums::Actions, bool> m_PreviousInput;
	std::map<Enums::Actions, bool> m_CurrentInput;
};

