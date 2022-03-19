#pragma once
#include "QuasartsEngine.h"
#include <string>


class MenuBarView : public Engine::GuiView
{
public:
	static std::string OpenFileDialogue();
public:
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
};
