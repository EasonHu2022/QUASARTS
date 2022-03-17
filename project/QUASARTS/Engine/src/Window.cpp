#include "Window.h"
#include "LogModule.h"


static void GLFWErrorCallback(int error, const char* description)
{
	QERROR("GLFW Error ({0}): {1}", error, description);
}

Window::Window(const WindowProps& props)
{

	Init(props);
}

Window::~Window()
{

	Shutdown();
}

Window* Window::Create(const WindowProps& props)
{
	//later change to unique ptr
	return new Window(props);
}

void Window::Init(const WindowProps& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	QDEBUG("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

	int success = glfwInit();
	if (success == GLFW_FALSE)
	{
		QERROR("Could not initialize GLFW!");
		return;
	}
		
	glfwSetErrorCallback(GLFWErrorCallback);


	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);

	glfwSwapInterval(1);
/*
	now reserve for event system
*/
#pragma region CallBacks
	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{

			switch (action)
			{
			case GLFW_PRESS:
			{
				break;
			}
			case GLFW_RELEASE:
			{
				break;
			}
			case GLFW_REPEAT:
			{
				break;
			}
			}
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				break;
			}
			case GLFW_RELEASE:
			{
				break;
			}
			}
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{

		});
#pragma endregion

	
}

void Window::Shutdown()
{
	
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

void Window::OnUpdate()
{
	glfwPollEvents();


	glfwSwapBuffers(m_Window);
}
