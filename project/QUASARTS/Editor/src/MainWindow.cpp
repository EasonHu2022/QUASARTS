#include "MainWindow.h"
#include <stdio.h>

static void glfw_error_callback(int error, const char* description)
{
	//later change it into log system
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int MainWindow::Init()
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	/*const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);*/

	//Create GLFW window with graphics context
	context = glfwCreateWindow(width, height, title, NULL, NULL);

	if (context == NULL)
		return 1;
	glfwMakeContextCurrent(context);
	glfwSwapInterval(1);


	return 0;
}

bool MainWindow::Update()
{
	if (glfwWindowShouldClose(context))
		return false;
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();
	int display_w, display_h;
	glfwGetFramebufferSize(context, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	////give a clear color of the mainwindow
	//glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);


	glfwSwapBuffers(context);
	return true;
}

void MainWindow::Release()
{
	glfwDestroyWindow(context);
	glfwTerminate();
}

const GLFWwindow* MainWindow::get_window_context()
{
	return context;
}