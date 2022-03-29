#pragma once
#include "QuasartsEngine.h"
#include "GuiViews/GuiView.h"
#include "filesystem"
#include <iostream>
class FileInputView : public GuiView
{
	void on_add() override;
	void on_gui() override;
	void on_remove() override;

};
