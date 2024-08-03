#pragma once
#include <vector>
class Texture;
class MenuWord;
class AudioSettings;

class OverworldSettings final
{
public:
	OverworldSettings(const Rectf& window);
	~OverworldSettings();

	OverworldSettings(OverworldSettings& rhs) = delete;
	OverworldSettings(OverworldSettings&& rhs) = delete;

	OverworldSettings& operator=(OverworldSettings& rhs) = delete;
	OverworldSettings& operator=(OverworldSettings&& rhs) = delete;

	void Draw() const;
	void Update(bool& paused);

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();

	bool ShouldExit();
	//bool ShouldGoBack();

private:
	const Rectf m_Window;

	Texture* m_pCardTexture;

	std::vector<MenuWord*> m_pWords;
	int m_Selected{};

	AudioSettings* m_pAudioSettings;

	bool m_AudioSettings{ false };
	bool m_ShouldGoBack{ false };
	bool m_Exit{ false };
};

