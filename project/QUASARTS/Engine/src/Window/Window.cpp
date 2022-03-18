#include "Window.h"
#include "Logger/LogModule.h"

#include <glad/glad.h>

static void GLFWErrorCallback(int error, const char* description)
{
	QERROR("GLFW Error ({0}): {1}", error, description);
}

Window::Window(const WindowProps& props)
{

	init(props);
}

Window::~Window()
{

	shutdown();
}

Window* Window::create(const WindowProps& props)
{
	//later change to unique ptr
	return new Window(props);
}

void Window::init(const WindowProps& props)
{
	m_Data.title = props.Title;
	m_Data.width = props.Width;
	m_Data.height = props.Height;

	QDEBUG("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

	

	int success = glfwInit();
	if (success == GLFW_FALSE)
	{
		QERROR("Could not initialize GLFW!");
		return;
	}
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(GLFWErrorCallback);


	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (status == 0)
	{
		QERROR("GLAD failed to load GL functions");
		return;
	}

	glfwSwapInterval(1);
/*
	now reserve for event system
*/
#pragma region CallBacks
	// Set GLFW callbacks
	//glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	//	{
	//     
	// 
	//	});

	//glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	//	{
	//    
	//	});

	//glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	//	{

	//		switch (action)
	//		{
	//		case GLFW_PRESS:
	//		{
	//			break;
	//		}
	//		case GLFW_RELEASE:
	//		{
	//			break;
	//		}
	//		case GLFW_REPEAT:
	//		{
	//			break;
	//		}
	//		}
	//	});

	//glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
	//	{
	//	});

	//glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
	//	{
	//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	//		switch (action)
	//		{
	//		case GLFW_PRESS:
	//		{
	//			break;
	//		}
	//		case GLFW_RELEASE:
	//		{
	//			break;
	//		}
	//		}
	//	});

	//glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	//	{
	//	});

	//glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
	//	{

	//	});
#pragma endregion

	
}

void Window::shutdown()
{
	


	glfwDestroyWindow(m_Window);

	glfwTerminate();


}

void Window::on_update()
{
	if (glfwWindowShouldClose(m_Window))
	{
		/*
			later instead of close event send to app
			now  just shutdown the window and can't close the app
			temply we 'll get an error after close window
			that's because we shutdown the imgui but app can't be shutdown, so the update is continued
			which cause an error in imgui
			just ignore it before we finish the event sys
		*/
		shutdown();
		return;
	}

	glfwPollEvents();

	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	glClearColor(1, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

}

void Window::swap_buffer()
{
	glfwSwapBuffers(m_Window);
}
