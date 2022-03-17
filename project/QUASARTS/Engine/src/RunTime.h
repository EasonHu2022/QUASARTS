#pragma once
/*
	the thing that actually running in the application
	for example: 
	 in edit mode: we have 1 application that is Quasarts Editor, and 2 runtime, 1 is editor and 1 is game;
	 in release: we have 1 application that is our game, and just 1 runtime that is game;

*/
class QS_API RunTime
{
public:
	RunTime();
	~RunTime();

	/*
		need to refer to a specific graphic context
		for example : in editor mode, editor runtime's context is the glfw window but game runtime's context is the scene view;
		later it will be implemented in render sys
	*/

	virtual void init() = 0;
	virtual void on_update() = 0;
	virtual void on_imgui() = 0;
	virtual void on_quit() = 0;

};

