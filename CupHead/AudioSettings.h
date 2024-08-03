#pragma once
class MenuWord;
class Texture;
#include <vector>

class AudioSettings final
{
public:
	AudioSettings(const Rectf& window);
	~AudioSettings();

	AudioSettings(AudioSettings& rhs) = delete;
	AudioSettings(AudioSettings&& rhs) = delete;

	AudioSettings& operator=(AudioSettings& rhs) = delete;
	AudioSettings& operator=(AudioSettings&& rhs) = delete;

	void Draw() const;

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();

	bool ShouldGoBack();

private:
	Texture* m_pCardTexture;
	const Rectf m_Window;

	std::vector<MenuWord*> m_pWords;
	int m_Selected{};

	MenuWord* m_pHyphen;
	MenuWord* m_pVerticalBar;

	bool m_ShouldGoBack{ false };

	void DrawBar(Point2f pos, int audioLevel, bool selected) const;

};

