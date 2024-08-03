#pragma once
#include "Enums.h"
#include <map>

class Controls
{
public:
	static void LoadControlsKeyboard();
	static void SaveControlsKeyboard();

	static void LoadControlsController();

	static int GetButton(Enums::Actions action);
	
	static ControllerButtonEntry GetControllerButton(Enums::Actions action);

private:
	static std::map<Enums::Actions, int> m_Controls;

	static std::map<Enums::Actions, ControllerButtonEntry> m_ControllerControls;
	
};