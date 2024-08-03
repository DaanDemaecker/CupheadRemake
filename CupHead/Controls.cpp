#include "pch.h"
#include "Controls.h"

std::map<Enums::Actions, int> Controls::m_Controls{};
std::map<Enums::Actions, ControllerButtonEntry> Controls::m_ControllerControls{};

void Controls::LoadControlsKeyboard()
{
	m_Controls[Enums::Actions::Up] = SDL_SCANCODE_W;
	m_Controls[Enums::Actions::Down] = SDL_SCANCODE_S;
	m_Controls[Enums::Actions::Left] = SDL_SCANCODE_A;
	m_Controls[Enums::Actions::Right] = SDL_SCANCODE_D;
	m_Controls[Enums::Actions::Jump] = SDL_SCANCODE_SPACE;
	m_Controls[Enums::Actions::Dash] = SDL_SCANCODE_LSHIFT;
	m_Controls[Enums::Actions::Aim] = SDL_SCANCODE_LCTRL;
	m_Controls[Enums::Actions::Shoot] = SDL_SCANCODE_U;
	m_Controls[Enums::Actions::Ex] = SDL_SCANCODE_I;
	m_Controls[Enums::Actions::SwapBullets] = SDL_SCANCODE_TAB;
}

void Controls::SaveControlsKeyboard()
{

}

void Controls::LoadControlsController()
{
	m_ControllerControls[Enums::Actions::Up] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_DPAD_UP);
	m_ControllerControls[Enums::Actions::Down] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	m_ControllerControls[Enums::Actions::Left] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	m_ControllerControls[Enums::Actions::Right] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	m_ControllerControls[Enums::Actions::Jump] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_A);
	m_ControllerControls[Enums::Actions::Dash] = ControllerButtonEntry(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	m_ControllerControls[Enums::Actions::Aim] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	m_ControllerControls[Enums::Actions::Shoot] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_X);
	m_ControllerControls[Enums::Actions::Ex] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_B);
	m_ControllerControls[Enums::Actions::SwapBullets] = ControllerButtonEntry(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
}

int Controls::GetButton(Enums::Actions action)
{
	return m_Controls[action];
}

ControllerButtonEntry Controls::GetControllerButton(Enums::Actions action)
{
	return m_ControllerControls.at(action);
}
