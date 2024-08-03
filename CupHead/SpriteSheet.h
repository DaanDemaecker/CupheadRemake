#pragma once
#include "Texture.h"
#include <string>

class SpriteSheet
{
//The Base SpriteSheet Class, all other spritesheet classes are derived from this one

public:
	explicit SpriteSheet(std::string textpath, int nrCols, int nrRows, int nrFrames, float frameSpeed = 1/20.f);

	SpriteSheet(SpriteSheet& rhs) = delete;
	SpriteSheet(SpriteSheet&& rhs) = delete;

	SpriteSheet& operator=(SpriteSheet& rhs) = delete;
	SpriteSheet& operator=(SpriteSheet&& rhs) = delete;


	virtual ~SpriteSheet();
	virtual void DrawPercentage(const Point2f& pos, float percentage = 1.f);
	virtual void DrawBottom(const Point2f& pos, bool facingRight = true, int mode = 0);
	virtual void DrawCenter(const Point2f& pos, bool facingRight = true, int mode = 0);
	virtual void DrawCenterRotated(const Point2f& pos, float angle, int mode = 0);
	virtual void Update(float elapsedSec);

	float GetFrameWidth() const;
	float GetFrameHeight() const;

	virtual int GetCrFrame();
	virtual void SetFrame(int frame);

protected:
	const float m_FrameSpeed;
	const int m_NrCols;
	const int m_NrRows;
	const int m_NrFrames;
	Texture* m_pTexture;

	int m_CrFrame{};
	float m_Timer{};

private:
	
	void NextFrame();
};

