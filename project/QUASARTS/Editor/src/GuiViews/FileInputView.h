#pragma once
#include "QuasartsEngine.h"
#include "GuiViews/GuiView.h"
#include "GuiViews/TextEditor.h"
#include "filesystem"
#include <iostream>
class FileInputView : public GuiView
{
	bool text_edit;
	TextEditor editor;
	std::string filePath;
	std::string fileText;
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
	void show_text();
};
