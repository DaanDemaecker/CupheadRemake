#pragma once
#include <vector>
#include <memory>

class Texture;

class Carrot final
{
public:

	Carrot();
	~Carrot();

	Carrot(Carrot& rhs) = delete;
	Carrot(Carrot&& rhs) = delete;
	Carrot& operator=(Carrot& rhs) = delete;
	Carrot& operator=(Carrot&& rhs) = delete;

	void DrawIntro() const;
	void Draw() const;
	void Update(float elapsedSec, const Rectf& shape, std::vector<std::weak_ptr<Point2f>>& targets);

	bool GetActivated();
	bool CheckHit(const Rectf& shape);
	bool CheckBulletHit(const Circlef& hitBox);

private:
	bool m_Activated{ true };

	float m_Angle{};
	float m_Speed{};
	float m_RotSpeed{};
	float m_Width;
	float m_Height;

	std::shared_ptr<Point2f> m_pCenter{};

	std::vector<Point2f> m_OriginalShape;
	std::vector<Point2f> m_HitBox;

	Texture* m_pTexture;
	Texture* m_pIntroTexture;

	const int m_NrCols;
	const int m_NrRows;
	const int m_NrFrames;
	
	float m_FrameSpeed{ 1 / 24.f };
	int m_CrFrame{};
	float m_Timer{};


	bool m_Intro{true};
	Point2f m_IntroPos{};

	void FixAngle(float& angle);

	void UpdateTimer(float elapsedSec);
};

