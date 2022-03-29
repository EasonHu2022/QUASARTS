#pragma once
#include "QuasartsEngine.h"

class GuiView
{
public:
	GuiView() {};
	~GuiView() {};
public:
	bool show_window = true;
	Window* window = Engine::Application::Instance->get_window();
	/*
		on gui update
	*/
	virtual void on_gui() = 0;
	
	/*
		on view added
	*/
	virtual  void on_add() = 0;

	/*
		on view removed
	*/
	virtual void on_remove() = 0;

private:

};

