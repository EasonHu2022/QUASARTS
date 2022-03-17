#pragma once
#include "QuasartsEngine.h"
#include "RunTime.h"
class EditorRuntime : public RunTime
{
public:
	EditorRuntime();
	~EditorRuntime();

	/*
		need to refer to a specific graphic context
		for example : in editor mode, editor runtime's context is the glfw window but game runtime's context is the scene view;
		later it will be implemented in render sys
	*/

	void init() override;
	void on_update() override;
	void on_imgui() override;
	void on_quit() override;

private:
	void _init_imgui();

	void _file_input();

	std::string _OpenFileDialogue();

	void _menubar();
};

