#pragma once
#include "QuasartsEngine.h"
class FileInputView : public Engine::GuiView
{
	void on_add() override;
	void on_gui() override;
	void on_remove() override;

};
