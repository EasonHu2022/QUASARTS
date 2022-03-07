#include "Renderer.h"
#include "LogModule.h"

//singleton
Renderer* Renderer::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
Renderer* Renderer::Instance()
{
	if (nullptr == instance)
		return new Renderer();
	else
	{
		return instance;
	}
}

static void glfw_error_callback(int error, const char* description)
{
	//later change it into log system
	QERROR("Glfw Error %d: %s\n", error, description);
}

/// <summary>
/// init
/// </summary>
int Renderer::init()
{
	/*according to the config
	init opengl correct version 
	or vulkan*/
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
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(status, "failed to init");
	glfwSwapInterval(1);
	return 0;
}

/// <summary>
/// start
/// </summary>
/// <returns>res</returns>
int Renderer::start()
{
	return 0;
}

/// <summary>
/// update
/// </summary>
int Renderer::render_loop()
{
	if (glfwWindowShouldClose(context))
		return 1;
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();
	int display_w, display_h;
	glfwGetFramebufferSize(context, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//give a clear color of the mainwindow
	//glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	//loop for render scene
	scene_loop();




	//loop for Editor UI
	editor_loop();




	glfwSwapBuffers(context);
	return 0;
}

/// <summary>
/// stop
/// </summary>
/// <returns>res</returns>
int Renderer::stop()
{
	return 0;
}

/// <summary>
/// release
/// </summary>
void Renderer::release()
{
	glfwDestroyWindow(context);
	glfwTerminate();
}

const GLFWwindow* Renderer::get_window_context()
{
	return context;
}

void Renderer::scene_loop()
{
}

void Renderer::editor_loop()
{
}