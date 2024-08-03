#pragma once
class Texture;
#include <vector>
#include <map>
#include "ScreenEffectSpriteSheet.h"

class ScreenEffects final
{
public:
	enum class State {
		TransitionIn = 0,
		TransitionOut = 1,
		Intro = 2,
		YouDied = 3,
		Knockout = 4,
		Idle = 5,
		Black = 6
	};

	explicit ScreenEffects(const Rectf& window);
	~ScreenEffects();

	ScreenEffects(ScreenEffects& rhs) = delete;
	ScreenEffects(ScreenEffects&& rhs) = delete;

	ScreenEffects& operator=(ScreenEffects& rhs) = delete;
	ScreenEffects& operator=(ScreenEffects&& rhs) = delete;


	void DrawFirstLayer();
	void DrawSecondLayer();
	void Update(float elapsedsec, bool paused);

	void Intro();
	void YouDied();
	void Knockout();
	void Idle();
	void Transition_Out();
	void Transition_In();

	void TwoStates(State firstState, State Secondstate);

	bool IsIdle();
	bool IsTransitioning();

private:
	const float m_FrameSpeed{ 1/20.f };
	const Rectf m_Window;
	Rectf m_YouDiedRect;

	State m_State{ State::Black};
	State m_NextState{ State::Idle };

	ScreenEffectSpriteSheet* m_pStaticTexture;

	std::map<State, ScreenEffectSpriteSheet*> m_pTextures{};

	void CheckState();
};
