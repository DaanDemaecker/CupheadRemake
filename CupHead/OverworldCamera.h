#pragma once

class OverworldCamera
{
public:
	OverworldCamera(const Rectf& window, const Rectf& boundaries);
	~OverworldCamera();

	OverworldCamera(OverworldCamera& rhs) = delete;
	OverworldCamera(OverworldCamera&& rhs) = delete;

	OverworldCamera& operator=(OverworldCamera& rhs) = delete;
	OverworldCamera& operator=(OverworldCamera&& rhs) = delete;

	void TranslateCamera(const Point2f& playerPos);

private:
	const Rectf m_Window;
	Rectf m_Boundaries;

	Point2f TrackPlayer(const Point2f& playerPos);
	void Clamp(Point2f& cameraPos);
};

