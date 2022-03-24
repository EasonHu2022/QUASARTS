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
		bIs_Running = false;


		LogModule::Instance()->init();

		EventModule::Instance()->init();

		loaderFactory = new MeshLoaderFactory();
		//create window for app
		m_window = Window::create(WindowProps(name));		
	}
	Application:: ~Application()
	{
		//do release things
		GuiWrapper::release();
	}

	/// <summary>
	/// init
	/// </summary>
	void Application::init()
	{
	
		/*
			later change to ECS mode
			later remove instance
		*/
		
		PhysicsManager::Instance()->init();
		


		//do init things
		GuiWrapper::init();
	}


	void Application::start()
	{

		bIs_Running = true;

		/*
			temp
		*/
		LogModule::Instance()->start();
		PhysicsManager::Instance()->start();
		EventModule::Instance()->start();

		/// <summary>
		/// for test
		/// </summary>
		Test();


		//main loop
		while (bIs_Running)
		{
			/*
				render frame
			*/			
			GuiWrapper::begin();
			{
				on_update();
				on_render();
			}
			GuiWrapper::end();

			/*
				System Manager Update
			*/
			m_window->swap_buffer();
		}

		//on destroy
		on_release();
	}

	void Application::on_render()
	{

		//do render things
	}

	void Application::on_gui()
	{
		//generally do nothing
	}

	void Application::on_update()
	{
		m_window->on_update();

		LogModule::Instance()->update();
		PhysicsManager::Instance()->update();
		EventModule::Instance()->update();

		on_gui();
	}


	void Application::close()
	{
		bIs_Running = false;
	}

	void Application::on_release()
	{
		
		LogModule::Instance()->release();
		PhysicsManager::Instance()->release();
		EventModule::Instance()->release();
		m_window->shutdown();
	}

}
