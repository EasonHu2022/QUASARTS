#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
namespace Engine
{
	
	Application::Application()
	{
	}
	Application:: ~Application()
	{

	}
	void Application::init()
	{
		//create window for app
		m_window = Window::Create(WindowProps(name));
	}
	void Application::start()
	{
		//do sth later
	}
	void Application::update()
	{
		for (auto rt : runtimes)
		{
			//it's for logic and render
			rt->on_update();
		}

		//haven't decided how ot call render sys
		//rendersys::OnUpdate();

		for (auto rt : runtimes)
		{
			//it's for logic and render
			rt->on_imgui();
		}


		m_window->OnUpdate();
	}
	void Application::stop()
	{
		for (auto rt : runtimes)
		{
			rt->on_quit();
		}
	}

	void Application::push_runtime(RunTime* runtime)
	{
		runtimes.push_back(runtime);
		//do sth maybe event
	}
	void Application::pop_runtime()
	{
		runtimes.pop_back();
		//do sth
	}

}
