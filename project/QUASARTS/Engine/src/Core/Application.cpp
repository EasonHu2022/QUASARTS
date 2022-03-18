#include "Application.h"
#include "Gui/GuiWrapper.h"
#include "Render/Renderer.h"
#include "Logger/LogModule.h"
#include "Scene/PhysicsManager.h"
#include "Event/EventModule.h"

namespace Engine
{
	Application* Application::Instance = NULL;
	
	Application::Application()
	{
		Instance = this;
		bIs_Running = true;

		/*
			later change to ECS mode
		*/
		LogModule::Instance()->init();
		PhysicsManager::Instance()->init();
		EventModule::Instance()->init();


		//create window for app
		m_window = Window::create(WindowProps(name));
		//do init things
		GuiWrapper::init();


		
		
	}
	Application:: ~Application()
	{
		//do release things
		GuiWrapper::release();
	}

	void Application::start()
	{
		/*
			temp
		*/
		LogModule::Instance()->start();
		PhysicsManager::Instance()->start();
		EventModule::Instance()->start();


		//main loop
		while (bIs_Running)
		{
			m_window->on_update();

			/*
				temp
			*/
			LogModule::Instance()->update();
			PhysicsManager::Instance()->update();
			EventModule::Instance()->update();


			/*
				Gui views
			*/
			
			GuiWrapper::begin();
			{
				GuiWrapper::Instance->on_gui();
			}
			GuiWrapper::end();

			/*
				System Manager Update
			*/




			m_window->swap_buffer();
		}
	}



	void Application::close()
	{
		bIs_Running = false;
		LogModule::Instance()->release();
		PhysicsManager::Instance()->release();
		EventModule::Instance()->release();
		m_window->shutdown();

	}
	int Application::add_gui_view(GuiView* view)
	{
		Engine::GuiWrapper::Instance->add_gui_view(view);
		return 0;
	}
	void Application::remove_gui_view(int handle)
	{
		Engine::GuiWrapper::Instance->remove_gui_view(handle);
	}
}
