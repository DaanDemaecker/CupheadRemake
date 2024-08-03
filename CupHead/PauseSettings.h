#pragma once
#include <vector>
class Texture;
class MenuWord;
class AudioSettings;

class PauseSettings final
{
public:
	PauseSettings(const Rectf& window);
	~PauseSettings();

	PauseSettings(PauseSettings& rhs) = delete;
	PauseSettings(PauseSettings&& rhs) = delete;

	PauseSettings& operator=(PauseSettings& rhs) = delete;
	PauseSettings& operator=(PauseSettings&& rhs) = delete;

	void Draw() const;
	void Update(bool& paused);

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();

	bool ShouldReset();
	bool ShouldExit();

private:
	const Rectf m_Window;

	Texture* m_pCardTexture;

	std::vector<MenuWord*> m_pWords;
	int m_Selected{};

	AudioSettings* m_pAudioSettings;

	bool m_AudioSettings{false};
	bool m_ShouldGoBack{ false };
	bool m_Reset{ false };
	bool m_Exit{ false };
};

