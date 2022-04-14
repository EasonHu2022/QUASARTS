#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"


class AttributeView : public GuiView
{
public:
	
	void on_add() override;
	void on_gui() override;
	void on_remove() override;

	bool game = false;

};
