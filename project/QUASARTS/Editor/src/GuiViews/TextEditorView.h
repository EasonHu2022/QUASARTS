#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"


class TextEditorView : public GuiView
{
public:
	bool text_edit;
	void on_add() override;
	void on_gui() override;
	void edit_text(bool value);
	void on_remove() override;
};
