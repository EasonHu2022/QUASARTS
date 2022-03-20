#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/FileInputView.h"


class MenuBarView : public GuiView
{
public:
	static std::string OpenFileDialogue();
public:
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
};
