#include "Window.h"
#include "Logger/LogModule.h"
#include "Event/EventModule.h"

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

	//do release things 
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
	{	
		using namespace Engine;
		
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				EventModule::Instance()->create_event("WindowResized", EventModule::EventPriority::High,
					{
						{ "width",		EV_ARG_INT(width)	},
						{ "height",		EV_ARG_INT(height)	}
					}
				);
			}
		);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{


				EventModule::Instance()->create_event("WindowClosed", EventModule::EventPriority::High, { });

				glfwDestroyWindow(window);

				glfwTerminate();
			}
		);

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					EventModule::Instance()->create_event("KeyPressed", EventModule::EventPriority::High,
						{
							{ "key",		EV_ARG_INT(keycode_convert_glfw_to_q(key))	},
							//{ "scancode",	EV_ARG_INT(scancode)	},
							{ "mods",		EV_ARG_INT(keymods_convert_glfw_to_q(mods))	},
							{ "repeat",		EV_ARG_BOOL(false)		}
						}
					);
					break;
				}
				case GLFW_RELEASE:
				{
					EventModule::Instance()->create_event("KeyReleased", EventModule::EventPriority::High,
						{
							{ "key",		EV_ARG_INT(keycode_convert_glfw_to_q(key))	},
							//{ "scancode",	EV_ARG_INT(scancode)	},
							{ "mods",		EV_ARG_INT(keymods_convert_glfw_to_q(mods))	}
						}
					);
					break;
				}
				case GLFW_REPEAT:
				{
					EventModule::Instance()->create_event("KeyPressed", EventModule::EventPriority::High,
						{
							{ "key",		EV_ARG_INT(keycode_convert_glfw_to_q(key))	},
							//{ "scancode",	EV_ARG_INT(scancode)	},
							{ "mods",		EV_ARG_INT(keymods_convert_glfw_to_q(mods))	},
							{ "repeat",		EV_ARG_BOOL(true)		}
						}
					);
					break;
				}
				}
			}
		);

		//glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		//	{
		//	});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				double xPos, yPos;
				glfwGetCursorPos(window, &xPos, &yPos);

				switch (action)
				{
				case GLFW_PRESS:
				{
					EventModule::Instance()->create_event("MouseButtonPressed", EventModule::EventPriority::High,
						{
							{ "button",		EV_ARG_INT(mousecode_convert_glfw_to_q(button))		},
							{ "mods",		EV_ARG_INT(keymods_convert_glfw_to_q(mods))			},
							{ "xpos",		EV_ARG_FLOAT((float)xPos)							},
							{ "ypos",		EV_ARG_FLOAT((float)yPos)							}
						}
					);
					break;
				}
				case GLFW_RELEASE:
				{
					EventModule::Instance()->create_event("MouseButtonReleased", EventModule::EventPriority::High,
						{
							{ "button",		EV_ARG_INT(mousecode_convert_glfw_to_q(button))		},
							{ "mods",		EV_ARG_INT(keymods_convert_glfw_to_q(mods))			},
							{ "xpos",		EV_ARG_FLOAT((float)xPos)							},
							{ "ypos",		EV_ARG_FLOAT((float)yPos)							}
						}
					);
					break;
				}
				}
			}
		);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				EventModule::Instance()->create_event("Scrolled", EventModule::EventPriority::High,
					{
						{ "xOffset",	EV_ARG_FLOAT(float(xOffset))	},
						{ "yOffset",	EV_ARG_FLOAT(float(yOffset))	}
					}
				);
			}
		);

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				EventModule::Instance()->create_event("MouseMoved", EventModule::EventPriority::High,
					{
						{ "xpos",		EV_ARG_FLOAT((float)xPos)	},
						{ "ypos",		EV_ARG_FLOAT((float)yPos)	}
					}
				);
			}
		);
	}
#pragma endregion

	
}


void Window::on_update()
{

	

	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	glClearColor(1, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	glfwPollEvents();

}

void Window::swap_buffer()
{
	glfwSwapBuffers(m_Window);
}
