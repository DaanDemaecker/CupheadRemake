#pragma once
#include <vector>
#include "Enums.h"
class Texture;
class MenuWord;
class AudioSettings;

class Settings final
{
public:
	Settings(const Rectf& window, Enums::Character cuphead);
	~Settings();

	Settings(Settings& rhs) = delete;
	Settings(Settings&& rhs) = delete;

	Settings& operator=(Settings& rhs) = delete;
	Settings& operator=(Settings&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec, Enums::Character& cupHead);

	bool ShouldGoBack();

	void Space();
	void Escape();
	void Up();
	void Right();
	void Down();
	void Left();

	enum class State
	{
		Main_Settings,
		Audio_Settings,
		Change_Character
	};

private:
	const Rectf m_Window;
	State m_State{ State::Main_Settings };

	AudioSettings* m_pAudioSettings;

	Texture* m_pCardTexture;
	std::vector<MenuWord*> m_pSettings1Words;
	int m_Selected1{};

	std::vector<MenuWord*> m_pSettings2Words;
	int m_Selected2{};

	bool m_GoBack{false};

	Enums::Character m_Cuphead;

	void MainSettingsSpace();

	void DrawMainSettings() const;
	void DrawChangeCharacter() const;

	
};

