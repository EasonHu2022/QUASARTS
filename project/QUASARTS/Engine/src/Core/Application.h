#pragma once
#include "core.h"
#include <vector>
#include "Window/Window.h"
#include "Gui/GuiWrapper.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
namespace Engine
{

	class QS_API Application
	{
	private:
		std::string name = "Quarsart Editor V1.0";
		Window* m_window = NULL;
		bool bIs_Running ;

	public:
		static Application* Instance;

	public:
		Window* get_window() { return m_window; }
		GLFWwindow* get_glfw_context() { return m_window->get_native_window(); }
		Application();
		virtual ~Application();
		virtual void start();
		virtual void close();
		int add_gui_view(Engine::GuiView* view);
		void remove_gui_view(int handle);
	};
	Application* create_application();
}


