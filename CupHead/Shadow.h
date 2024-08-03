#pragma once
class Texture;

class Shadow
{
public:
	Shadow();
	~Shadow();

	Shadow(Shadow& other) = delete;
	Shadow(Shadow&& other) = delete;

	Shadow& operator=(Shadow& other) = delete;
	Shadow& operator=(Shadow&& other) = delete;

	void Draw() const;
	void Update(const Point2f& playerPos, const Point2f& goundPos);

private:
	Texture* m_pTexture;

	const int m_NrCols;
	const int m_NrRows;
	const int m_NrFrames;
	int m_Frame{};

	Point2f m_GroundPos{};
	const float m_MaxDistance;
};

