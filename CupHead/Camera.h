#pragma once
class Camera final
{
public:
	explicit Camera(const Rectf& window);

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void TranslateMiddleground(float factor) const;
	void TranslateForeground(float yOffset = 0) const;
	void Update(float elapsedSec, const Rectf& target, float velocityX);

	Point2f GetCameraPos();
private:
	const Rectf m_Window;

	Rectf m_LevelBoundaries;

	Point2f m_CameraPos;
	float m_Speed;

	const float m_MaxSpeed{440.f};
	const float m_Acceleration{800.f};

	void ClampOnPlayer(Point2f& cameraPos, const Rectf& target);
	void ClampOnLevel(Point2f& cameraPos, const Rectf& target);
};

