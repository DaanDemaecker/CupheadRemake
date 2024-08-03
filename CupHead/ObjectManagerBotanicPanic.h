#include "ObjectManager.h"
#include <vector>
#include "Vector2f.h"
class Object;
class Level;
class Pyramid;
class Bullet;
class BigBullet;
class SalSpudder;
class OllieBulb;
class ChaunceyChantenay;
class HoraceRadiche;

class ObjectManagerBotanicPanic final : public ObjectManager
{
public:
	ObjectManagerBotanicPanic();
	virtual ~ObjectManagerBotanicPanic() override;

	virtual void DrawLevel1() const override;

	virtual void DrawLevel3() const override;

	virtual void DrawLevel4() const override;

	virtual void Update(float elapsedSec, const Level* level, const Rectf& shape) override;
	virtual void FixParallax(const Point2f& cameraDifference) override;

	virtual bool CheckParry(const Rectf& shape, float damage) override;
	virtual bool Checkhits(Rectf& shape, bool parry, Vector2f& velocity) override;

	virtual float CheckBullets(std::vector<Bullet*>& bullets, std::vector<BigBullet*>& bigBullets) override;

	bool IsGameOver();

	virtual LoseScreenData GetLoseScreenData() override;
private:
	SalSpudder* m_pSalSpudder;
	OllieBulb* m_pOllieBulb;
	ChaunceyChantenay* m_pChauncyChantenay;

	enum class State
	{
		salSpudder = 0,
		ollieBulb = 1,
		chaunceyChantenay = 2,
		knockout = 3
	};
	
	State m_State{ State::salSpudder };

	HoraceRadiche* m_pHoraceRadiche;
	bool m_RadishActive{ false };
	bool m_RadishDefeated{ false };
};

