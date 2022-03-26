#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"

class GameSceneView : public GuiView
{
public:
	bool new_project;
	std::string folder_path;
	static std::string OpenFileDialogue();
	static std::string OpenFolderDialogue();
	void newProject();
public:
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
};
