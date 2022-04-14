#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/FileInputView.h"

class MenuBarView : public GuiView
{
public:
	bool new_project;
	bool new_scene;
	bool new_script;
	bool new_entity;
	bool new_child;
	std::string folder_path;
	static std::string OpenFileDialogue();
	static std::string OpenFolderDialogue();
	void newProject();
	void newScene();
	void newScript();
	void newEntity();
	void newChild();
public:
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
};
