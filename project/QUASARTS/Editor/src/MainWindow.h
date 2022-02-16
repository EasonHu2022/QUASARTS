#pragma once
#include "glfw3.h"
class MainWindow
{
public:
	int Init();
	bool Update();
	void Release();
public:
	const GLFWwindow* get_window_context();
private:
	GLFWwindow* context;
	int width = 1280;
	int height = 720;

	char* title = "QUASARTS Editor";
};
